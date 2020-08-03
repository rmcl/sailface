#include "sailface.h"
#include "helm.h"


void SailFaceCommunication::initialize(SailFaceStatus *status) {

}

void SailFaceCommunication::poll(SailFaceStatus *status) {
    // send data only when you receive data:
    if (Serial.available() > 0) {
        // read the incoming byte:
        incomingByte = Serial.read();

        // say what you got:
        Serial.print("I received: ");
        Serial.println(incomingByte);
    }

    void loop()
{

static char inputBuffer [MAX_INPUT];
static unsigned int input_pos = 0;


void SailFaceCommunication::readCommandFromSerial() {
    if (Serial.available () > 0) {
        char inByte = Serial.read();

        switch (inByte) {
            case '\n':   // end of text
                input_line[input_pos] = 0;  // terminating null byte

                // terminator reached! process input_line here ...
                process_data (input_line);

                // reset buffer for next time
                input_pos = 0;
                break;

            case '\r':   // discard carriage return
                break;

            default:
                // keep adding if not full ... allow for terminating null byte
                if (input_pos < (MAX_INPUT - 1))
                    input_line [input_pos++] = inByte;
                    break;

        }

    }
}
