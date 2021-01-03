/*

The goal of this module is to send and receive messages via the Iridium Short-Burst Data services
Sending messages to/from satellites is expensive so we can mock this communication with a bluetooth serial port.

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

void SailFaceCommunication::poll(SailFaceStatus *status) {
    //char *message = readMessageFromSerial();

    // Write teh SailFace status structure to the bluetooth serial port.
    bluetoothSerial.write( (byte *) status, sizeof(status));

}

void sendDebugMessage(char *message) {
    bluetoothSerial.write(message);
}


/*

*/
char* SailFaceCommunication::readMessageFromSerial() {
    while (bluetoothSerial.available() > 0) {
        char inByte = Serial.read();

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
    return 0;
}
