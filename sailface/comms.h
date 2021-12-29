#ifndef _SAIL_FACE_COMMS
#define _SAIL_FACE_COMMS

#include <stdint.h>
#include <Arduino.h>

#include "sailface.h"

#define COMM_MAX_COMMAND_LEN 100


class SailFaceCommunication {

    private:
        char inputBuffer [COMM_MAX_COMMAND_LEN];
        unsigned int inputBufferPosition;
        bool inputBufferReady;

        // TODO DOCUMENT THE CORRECT ARDUINO MEGA PINS HERE!
        //SoftwareSerial bluetoothSerial{COMM_BLUETOOTH_RX_PIN, COMM_BLUETOOTH_TX_PIN}; // RX | TX
        HardwareSerial &bluetoothSerial = Serial2;

        char *readMessageFromSerial();

    public:
        void initialize(SailFaceStatus *status);
        char *pollForCommandMessages(SailFaceStatus *status);
        void sendDebugMessage(char *message);

};
#endif
