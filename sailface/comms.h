#ifndef _SAIL_FACE_COMMS
#define _SAIL_FACE_COMMS

#include <stdint.h>
#include <Arduino.h>
#include <IridiumSBD.h>

#define COMM_MAX_COMMAND_LEN 100

#define ROCKBLOCK_SLEEP_PIN 40
#define ROCKBLOCK_RING_PIN 38

typedef struct {

  long latitude;
  long longitude;

  long waypointLatitude;
  long waypointLongitude;

  short batteryVoltage;

  // speed of prop between 0 and 255. Set to -1 for no change.
  short propSpeed;

} SailFaceIridiumStatusMessage;

// Structure of messages received from Iridium.
typedef struct {

  // Desired Waypoint
  // Set to 0 for no change.
  long waypointLatitude;
  long waypointLongitude;

  // speed of prop between 0 and 255. Set to -1 for no change.
  int propSpeed;

  bool bluetoothActive;

  // Time in seconds that status updates are sent through Iridium.
  uint32_t iridiumUpdateFrequencySeconds;

} SailFaceCommandMessage;

/* Radio Control Related */

#define RADIO_CONTROL_PROP_SPEED_PIN 11
#define RADIO_CONTROL_RUDDER_CONTROL_PIN 10

// Structure of messages received from the RC controller.
typedef struct {
  // speed of prop between 0 and 255. Set to -1 for no change.
  int propSpeed;
  int rudderPosition;

} SailFaceRadioCommandMessage;

/* End Radio Control Related */

class CommunicationManager {

    private:
        bool radioControlActive;
        bool iridiumActive;

        int iridiumSignalQuality;


        // Serial3 on the Mega is pins TX3 pin 14 and RX3 pin 15
        // Iridium RXD pin1 -> RX3 pin 15
        // Iridium TXD pin6 -> TX3 pin 14
        HardwareSerial &IridiumSerial = Serial3;
        IridiumSBD modem{
          IridiumSerial,
          ROCKBLOCK_SLEEP_PIN,
          ROCKBLOCK_RING_PIN
        };

        char *readMessageFromBluetooth();
        int pollForIridiumRingAlerts();
        int retieveIridiumMessage(SailFaceCommandMessage message);
        void initializeIridium();
        SailFaceIridiumStatusMessage buildStatusMessage(
            long curBatteryVoltage,
            long curLatitude,
            long curLongitude,
            long curWaypointLat,
            long curWaypointLong,
            long curPropSpeed
        );


    public:
        void initialize();

        void wakeIridium();
        void sleepIridium();

        void pollIridiumSignalQuality();
        char *pollForBluetoothCommandMessages();
        void pollForCurrentRadioCommand(SailFaceRadioCommandMessage *radioCommand);

        int pollForIridumCommandMessages(
            SailFaceIridiumStatusMessage statusMessage,
            SailFaceCommandMessage *firstReceivedCommand,
            bool sendStatusMessage
        );
        int sendReceiveIridiumStatusCommandMessage(
          SailFaceIridiumStatusMessage *txMessage,
          SailFaceCommandMessage *rxCommandMessage
        );

        void sendBluetoothMessage(String *message);
        HardwareSerial *getBluetoothSerial();
};
#endif
