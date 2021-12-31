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

}

void SailFaceCommunication::initializeIridium(SailFaceStatus *status) {
    int errorCode = -1;

    IridiumSerial.begin(19200);
    modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE);

    // Begin satellite modem operation
    errorCode = modem.begin();
    if (errorCode != ISBD_SUCCESS) {
        Serial.print("Begin failed: error ");
        Serial.println(errorCode);
        if (err == ISBD_NO_MODEM_DETECTED) {
            Serial.println("No modem detected: check wiring.");
        }
        return;
    }

    errorCode = modem.getSignalQuality(status->iridiumSignalQuality);
    if (errorCode != ISBD_SUCCESS) {
        Serial.print("SignalQuality failed: error ");
        Serial.println(errorCode);
        return;
    }

    status->iridiumActive = true;
}

// The IridiumSBD will set the "Ring" pin to high if their are messages
// waiting for download. Return the number of messages waiting for
// download.
int SailFaceCommunication::pollForRingAlerts() {
    bool ring = modem.hasRingAsserted();
    if (ring) {
        return modem.getWaitingMessageCount();
    }
    return 0;
}

int SailFaceCommunication::retieveMessage() {
    uint8_t buffer[200];
    size_t bufferSize = sizeof(buffer);
    err = modem.sendReceiveSBDText(NULL, buffer, bufferSize);
    if (err != ISBD_SUCCESS) {
        Serial.print("sendReceiveSBDBinary failed: error ");
        Serial.println(err);
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
    int err = modem.sendSBDBinary(status, sizeof(SailFaceStatus))

    if (err != ISBD_SUCCESS) {
        Serial.print("sendSBDText failed: error ");
        Serial.println(err);

        if (err == ISBD_SENDRECEIVE_TIMEOUT) {
            Serial.println("Try again with a better view of the sky.");
        }
    } else {
        Serial.println("Hey, it worked!");

        // FIGURE OUT WHAT TO SET THIS TOO!
        //status->lastStatusMessageTime = 
    }
}

SailFaceCommandMessage SailFaceCommunication::pollForIridumCommandMessages(SailFaceStatus *status) {
    if (!status->iridiumSBDActive) {
        return NULL;
    }

    int messageCount = pollForRingAlerts();
    if (messageCount == 0) {
        return NULL;
    }

    return retieveMessage();
}



char *SailFaceCommunication::pollForBluetoothCommandMessages(SailFaceStatus *status) {
    char *message = readMessageFromBluetooth();
    if (message[0] != 0) {
        return message;
    }

    // no command messages available
    return NULL;
}

void SailFaceCommunication::sendDebugMessage(char *message) {
    bluetoothSerial.write(message);
}


/*

*/
char* SailFaceCommunication::readMessageFromBluetooth() {
    String command = "";

    /*
    //switch to listening on this software serial
    //bluetoothSerial.listen();
    if (!bluetoothSerial.isListening()) {
        //Serial.println("bluetooth is not listening");
        bluetoothSerial.listen();
        delay(100);
    }*/

    while (bluetoothSerial.available() > 0) {
        char inByte = bluetoothSerial.read();

        switch (inByte) {
            case '\n':   // end of text
                inputBuffer[inputBufferPosition] = 0;  // terminating null byte

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
