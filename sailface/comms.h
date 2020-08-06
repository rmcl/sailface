#ifndef _SAIL_FACE_COMMS
#define _SAIL_FACE_COMMS

#include <stdint.h>
#include <Arduino.h>

#include "sailface.h"

#define COMM_MAX_COMMAND_LEN 100

class SailFaceCommunication {

    private:
        static char inputBuffer [COMM_MAX_COMMAND_LEN];
        static unsigned int inputBufferPosition;
        
        char *readMessageFromSerial();

    public:
        void initialize(SailFaceStatus *status);
        void poll(SailFaceStatus *status);

};
#endif
