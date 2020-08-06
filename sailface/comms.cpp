#include "sailface.h"
#include "comms.h"

/* The goal of this module is to send and receive messages via the Iridium Short-Burst Data services
Sending messages to/from satellites is expensive so we can mock this communication with the serial port.
*/


void SailFaceCommunication::initialize(SailFaceStatus *status) {
    inputBufferPosition = 0;
}

void SailFaceCommunication::poll(SailFaceStatus *status) {
    char *message = readMessageFromSerial();

}
/*

*/
char* SailFaceCommunication::readMessageFromSerial() {
    if (Serial.available() > 0) {
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
