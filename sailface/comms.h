#ifndef _SAIL_FACE_COMMS
#define _SAIL_FACE_COMMS

#include <stdint.h>
#include <Arduino.h>

#include "sailface.h"

#define COMM_MAX_COMMAND_LEN 100

// Structure of messages received from Iridium.
typedef struct {

    // Desired Waypoint
    // Set to 0 for no change.
    double waypointLatitude;
    double waypointLongitude;

    // speed of prop between 0 and 255. Set to -1 for no change.
    int propSpeed;

    bool bluetoothActive;

    // Time in seconds that status updates are sent through Iridium.
    uint32_t iridiumUpdateFrequencySeconds;

} SailFaceCommandMessage;


class SailFaceCommunication {

    private:
        char inputBuffer [COMM_MAX_COMMAND_LEN];
        unsigned int inputBufferPosition;
        bool inputBufferReady;

        // Serial2 on the Mega is pins TX2 pin 16 and RX2 pin 17
        // Bluetooth RXD -> TX2 pin 16
        // Bluetooth TXD -> RX2 pin 17
        HardwareSerial &bluetoothSerial = Serial2;

        // Serial3 on the Mega is pins TX3 pin 14 and RX3 pin 15
        // Iridium RXD pin1 -> RX3 pin 15
        // Iridium TXD pin6 -> TX3 pin 14
        HardwareSerial &IridiumSerial = Serial3;
        IridiumSBD modem(IridiumSerial);


        char *readMessageFromBluetooth();

    public:
        void initialize(SailFaceStatus *status);
        void initializeIridium(SailFaceStatus *status);
        char *pollForBluetoothCommandMessages(SailFaceStatus *status);
        SailFaceCommandMessage pollForIridumCommandMessages(SailFaceStatus *status);
        void sendDebugMessage(char *message);

};
#endif
