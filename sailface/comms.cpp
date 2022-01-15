/*

The goal of this module is to send and receive messages via the Iridium Short-Burst
Data services. Sending messages to/from satellites is expensive so we can mock
this communication with a bluetooth serial port.

Default Baud for bluetooth comms
Connect to HC-05 from any other bluetooth device with 1234 as pairing key!

Helpful links:
https://www.instructables.com/Setting-Up-Bluetooth-HC-05-With-Arduino/

*/

#include "sailface.h"
#include "comms.h"

void SailFaceCommunication::initialize(SailFaceStatus *status) {
    inputBufferPosition = 0;

    bluetoothSerial.begin(9600);

    // Set radio contorl pins for INPUT.
    pinMode(RADIO_CONTROL_PROP_SPEED_PIN, INPUT);
    pinMode(RADIO_CONTROL_RUDDER_CONTROL_PIN, INPUT);

    status->bluetoothActive = true;
    status->radioControlActive = false;

    startIridium(status);
}

void SailFaceCommunication::startIridium(SailFaceStatus *status) {
    int errorCode = -1;

    // SET THE SLEEP PIN TO OUTPUT
    pinMode(ROCKBLOCK_SLEEP_PIN, OUTPUT);
    digitalWrite(ROCKBLOCK_SLEEP_PIN, HIGH);

    logDebugMessage("Attempting to start iridium rockblock.");

    IridiumSerial.begin(19200);
    modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE);
    modem.adjustATTimeout(45);

    // Begin satellite modem operation
    errorCode = modem.begin();
    if (errorCode != ISBD_SUCCESS) {
        logDebugMessage("Begin failed: error ");
        logDebugMessage(errorCode);
        logDebugMessage("\n");
        if (errorCode == ISBD_NO_MODEM_DETECTED) {
            logDebugMessage("No modem detected: check wiring.");
        }

        status->iridiumActive = false;
    } else {

        // TODO: DO WE WANT THIS!?
        // move this somewhere! SHOULD WE REALLY TURN THIS OFF? OR IS IT ALREADY OFF?
        modem.useMSSTMWorkaround(false);

        pollIridiumSignalQuality(status);
        status->iridiumActive = true;
    }
}

void SailFaceCommunication::sleepIridium(SailFaceStatus *status) {
    digitalWrite(ROCKBLOCK_SLEEP_PIN, LOW);
    status->iridiumActive = false;
}

void SailFaceCommunication::pollIridiumSignalQuality(SailFaceStatus *status) {
    int errorCode = modem.getSignalQuality(status->iridiumSignalQuality);
    if (errorCode != ISBD_SUCCESS) {
        logDebugMessage("SignalQuality failed: error ");
        logDebugMessage(errorCode);
        logDebugMessage("\n");
        return;
    }
}

// The IridiumSBD will set the "Ring" pin to high if their are messages
// waiting for download. Return the number of messages waiting for
// download.
int SailFaceCommunication::pollForIridiumRingAlerts() {
    bool ring = modem.hasRingAsserted();
    if (ring) {
        return modem.getWaitingMessageCount();
    }
    return 0;
}

// Retrieve a message from Iridium and process it into a SailFaceCommandMessage
// Return the number of messages remaining for download.
int SailFaceCommunication::retieveIridiumMessage(SailFaceCommandMessage message) {
    uint8_t buffer[200];
    size_t bufferSize = sizeof(buffer);
    int errorCode = modem.sendReceiveSBDText(NULL, buffer, bufferSize);
    if (errorCode != ISBD_SUCCESS) {
        Serial.print("sendReceiveSBDBinary failed: error ");
        Serial.println(errorCode);
        return;
    }

    Serial.println("Message received!");
    Serial.print("Inbound message size is ");
    Serial.println(bufferSize);
    for (int i=0; i<(int)bufferSize; ++i) {
        Serial.print(buffer[i], HEX);
        if (isprint(buffer[i])) {
            Serial.print("(");
            Serial.write(buffer[i]);
            Serial.print(")");
        }
        Serial.print(" ");
    }
    Serial.println();
    Serial.print("Messages remaining to be retrieved: ");
    Serial.println(modem.getWaitingMessageCount());

}

void SailFaceCommunication::sendIridiumStatusMessage(SailFaceStatus *status) {

    SailFaceIridiumStatusMessage message;
    message.batteryVoltage = status->batteryVoltage * 10; // multiple by ten to convert to short int
    message.latitude = status->latitude;
    message.longitude = status->longitude;
    message.waypointLatitude = status->waypointLatitude;
    message.waypointLongitude = status->waypointLongitude;
    message.propSpeed = status->propSpeed;

    int errorCode = modem.sendSBDBinary((uint8_t*)&message, sizeof(message));
    if (errorCode != ISBD_SUCCESS) {
        sendDebugMessage("sendSBDText failed error:");
        char numberMessage[16];
        itoa(errorCode, numberMessage, 10);
        sendDebugMessage(numberMessage);
        sendDebugMessage("\n");

        if (errorCode == ISBD_SENDRECEIVE_TIMEOUT) {
            sendDebugMessage("Try again with a better view of the sky.");
        }

    } else {
        sendDebugMessage("Iridium successfully transmitted message.");
        status->lastIridiumStatusMessageSentTime = status->time;
    }
}

int SailFaceCommunication::pollForIridumCommandMessages(SailFaceStatus *status, SailFaceCommandMessage *firstReceivedCommand) {
    if (!status->iridiumActive) {
        return -1;
    }

    int messageCount = pollForIridiumRingAlerts();
    if (messageCount == 0) {
        return 0;
    }

    messageCount = retieveIridiumMessage(*firstReceivedCommand);

    // Current message + some number of outstanding messages.
    return 1 + messageCount;
}



char *SailFaceCommunication::pollForBluetoothCommandMessages(SailFaceStatus *status) {
    char *message = readMessageFromBluetooth();
    if (message[0] != 0) {
        return message;
    }

    // no command messages available
    return NULL;
}

//
// Send a debug message over the appropriate channel.
void SailFaceCommunication::sendDebugMessage(char *message) {
    bluetoothSerial.write(message);
    // we could fall back to good ol' Serial.print here.
}

void SailFaceCommunication::writeStatusMessage(SailFaceStatus *status) {
    bluetoothSerial.write("R");
    bluetoothSerial.write((byte*)status, sizeof(*status));
    bluetoothSerial.write("\n");
}

/*

*/
char* SailFaceCommunication::readMessageFromBluetooth() {

    while (bluetoothSerial.available() > 0) {
        char inByte = bluetoothSerial.read();

        switch (inByte) {
            case '\n':   // end of text
                inputBuffer[inputBufferPosition] = '\0';  // terminating null byte

                // reset buffer for next time
                inputBufferPosition = 0;

                // return the retrieved command.
                return inputBuffer;

            case '\r':   // discard carriage return
                break;

            default:
                // keep adding if not full ... allow for terminating null byte
                if (inputBufferPosition < (COMM_MAX_COMMAND_LEN - 1)) {
                    inputBuffer[inputBufferPosition] = inByte;
                    inputBufferPosition++;
                    break;
                }
        }
    }
    return '\0';
}

//
//
// Source: https://www.youtube.com/watch?v=Bx0y1qyLHQ4
//
void SailFaceCommunication::pollForCurrentRadioCommand(SailFaceRadioCommandMessage *radioCommand) {
    int propSpeedPulseWidth = pulseIn(
        RADIO_CONTROL_PROP_SPEED_PIN, HIGH);
    int rudderPositionPulseWidth = pulseIn(
        RADIO_CONTROL_RUDDER_CONTROL_PIN, HIGH);

    if (propSpeedPulseWidth < 1000) {
        radioCommand->propSpeed = -1;
        radioCommand->rudderPosition = 0;
    } else {
        int propSpeed = map(propSpeedPulseWidth, 1000, 2000, -20, 255);
        radioCommand->propSpeed = constrain(propSpeed, 0, 255);

        int rudderPosition = map(rudderPositionPulseWidth, 1000, 2000, -10, 10);
        radioCommand->rudderPosition = constrain(rudderPosition, -10, 10);
    }
}


void ISBDConsoleCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}
