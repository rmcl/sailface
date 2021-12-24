#ifndef _SAIL_FACE_COMMS
#define _SAIL_FACE_COMMS

#include <stdint.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#include "sailface.h"

#define COMM_MAX_COMMAND_LEN 100
#define COMM_BLUETOOTH_RX_PIN 10
#define COMM_BLUETOOTH_TX_PIN 11


class SailFaceCommunication {

    private:
        char inputBuffer [COMM_MAX_COMMAND_LEN];
        unsigned int inputBufferPosition;

        SoftwareSerial bluetoothSerial{COMM_BLUETOOTH_RX_PIN, COMM_BLUETOOTH_TX_PIN}; // RX | TX

        char *readMessageFromSerial();

    public:
        void initialize(SailFaceStatus *status);
        void poll(SailFaceStatus *status);
        void sendDebugMessage(char *message);

};
#endif
