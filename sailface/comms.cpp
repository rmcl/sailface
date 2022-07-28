/*

The goal of this module is to send and receive messages via the Iridium Short-Burst
Data services. Sending messages to/from satellites is expensive so we can mock
this communication with a bluetooth serial port.

Default Baud for bluetooth comms
Connect to HC-05 from any other bluetooth device with 1234 as pairing key!

Helpful links:
https://www.instructables.com/Setting-Up-Bluetooth-HC-05-With-Arduino/

*/

#include "comms.h"

void CommunicationManager::initialize() {

    // Set radio contorl pins for INPUT.
    pinMode(RADIO_CONTROL_PROP_SPEED_PIN, INPUT);
    pinMode(RADIO_CONTROL_RUDDER_CONTROL_PIN, INPUT);

    radioControlActive = false;

    //initializeIridium(status);
}

/*
* Iridium Specific Code
*
*/


void CommunicationManager::initializeIridium() {
    int errorCode = -1;

    // SET THE SLEEP PIN TO OUTPUT
    pinMode(ROCKBLOCK_SLEEP_PIN, OUTPUT);
    digitalWrite(ROCKBLOCK_SLEEP_PIN, HIGH);

    // Setup the Ring Indicator Pin
    pinMode(ROCKBLOCK_RING_PIN, INPUT);

    IridiumSerial.begin(19200);
    modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE);
    modem.adjustATTimeout(45);

    // Begin satellite modem operation
    errorCode = modem.begin();
    if (errorCode != ISBD_SUCCESS) {
        /*logDebugMessage("Begin failed: error ");
        logDebugMessage(errorCode);
        logDebugMessage("\n");
        if (errorCode == ISBD_NO_MODEM_DETECTED) {
            logDebugMessage("No modem detected: check wiring.");
        }
        */

        iridiumActive = false;
    } else {

        // TODO: DO WE WANT THIS!?
        // move this somewhere! SHOULD WE REALLY TURN THIS OFF? OR IS IT ALREADY OFF?
        modem.useMSSTMWorkaround(false);

        pollIridiumSignalQuality();
        iridiumActive = true;
    }
}

void CommunicationManager::wakeIridium() {
    digitalWrite(ROCKBLOCK_SLEEP_PIN, HIGH);
    iridiumActive = true;
}

void CommunicationManager::sleepIridium() {
    digitalWrite(ROCKBLOCK_SLEEP_PIN, LOW);
    iridiumActive = false;
}

void CommunicationManager::pollIridiumSignalQuality() {
    int errorCode = modem.getSignalQuality(iridiumSignalQuality);
    if (errorCode != ISBD_SUCCESS) {
        /*logDebugMessage("SignalQuality failed: error ");
        logDebugMessage(errorCode);
        logDebugMessage("\n");*/
        return;
    }
}

// The IridiumSBD will set the "Ring" pin to high if their are messages
// waiting for download. Return the number of messages waiting for
// download.
int CommunicationManager::pollForIridiumRingAlerts() {
    bool ring = modem.hasRingAsserted();
    if (ring) {
        return modem.getWaitingMessageCount();
    }
    return 0;
}


SailFaceIridiumStatusMessage CommunicationManager::buildStatusMessage(
    long curBatteryVoltage,
    long curLatitude,
    long curLongitude,
    long curWaypointLat,
    long curWaypointLong,
    long curPropSpeed

) {
    SailFaceIridiumStatusMessage message;
    message.batteryVoltage = curBatteryVoltage * 10; // multiple by ten to convert to short int
    message.latitude = curLatitude;
    message.longitude = curLongitude;
    message.waypointLatitude = curWaypointLat;
    message.waypointLongitude = curWaypointLong;
    message.propSpeed = curPropSpeed;
    return message;
}

//
// Send and Receive messages from Iridium.
int CommunicationManager::sendReceiveIridiumStatusCommandMessage(
    SailFaceIridiumStatusMessage *txMessage,
    SailFaceCommandMessage *rxCommandMessage
) {

    int txMessageSize = 0;
    if (txMessage != NULL) {
        txMessageSize = sizeof(*txMessage);
    }
    int rxMessageSize = sizeof(*rxCommandMessage);

    int errorCode = -1;
    /*
    int errorCode = modem.sendReceiveSBDBinary(
        (uint8_t*)txMessage,
        txMessageSize,
        (uint8_t*)rxCommandMessage,
        &rxMessageSize);
    */

    if (errorCode != ISBD_SUCCESS) {
        //sendDebugMessage("sendReceiveSBDBinary failed error:");
        char numberMessage[16];
        itoa(errorCode, numberMessage, 10);
        //sendDebugMessage(numberMessage);
        //sendDebugMessage("\n");

        if (errorCode == ISBD_SENDRECEIVE_TIMEOUT) {
            //sendDebugMessage("Try again with a better view of the sky.");
        }

        return -1;

    } else {
        //sendDebugMessage("Iridium successfully sent/recv message.");
        //status->lastIridiumStatusMessageSentTime = status->time;

        int messageCount = modem.getWaitingMessageCount();
        if (rxMessageSize > 0) {
            messageCount += 1;
        }

        return messageCount;
    }
}

// Poll for messages via the RockBlock Ring Alert Pin
// If send status message is True, then send status message and also check for
// waiting messages after the send.
// if command message received populate it into firstReceivedCommand and return
// the number of messages waiting INCLUDING the just returned message.
int CommunicationManager::pollForIridumCommandMessages(
    SailFaceIridiumStatusMessage statusMessage,
    SailFaceCommandMessage *firstReceivedCommand,
    bool sendStatusMessage
) {
    if (iridiumActive) {
        return -1;
    }

    int messageCount = pollForIridiumRingAlerts();
    if (messageCount == 0 && sendStatusMessage == false) {
        return 0;
    }

    SailFaceIridiumStatusMessage *txStatusMessagePtr = &statusMessage;

    //messageCount = sendReceiveIridiumStatusCommandMessage(
    //    txStatusMessagePtr,
    //    firstReceivedCommand);

    // Current message + some number of outstanding messages.
    return 1 + messageCount;
}


/*
char *CommunicationManager::pollForBluetoothCommandMessages() {
    char *message = readMessageFromBluetooth();
    if (message[0] != 0) {
        return message;
    }

    // no command messages available
    return NULL;
}

//
// Send a debug message over the appropriate channel.
void CommunicationManager::sendBluetoothMessage(String *message) {
    //bluetoothSerial.print(message);
}

HardwareSerial *CommunicationManager::getBluetoothSerial() {
    return &bluetoothSerial;
}



char* CommunicationManager::readMessageFromBluetooth() {

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
*/
//
//
// Source: https://www.youtube.com/watch?v=Bx0y1qyLHQ4
//
void CommunicationManager::pollForCurrentRadioCommand(SailFaceRadioCommandMessage *radioCommand) {
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
  //Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{
  //Serial.write(c);
}
