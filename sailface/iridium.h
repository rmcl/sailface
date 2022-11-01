#ifndef _IRIDIUM_COMMS
#define _IRIDIUM_COMMS

#include <stdint.h>
#include <Arduino.h>
#include <IridiumSBD.h>
#include "navigation.h"

#define COMM_MAX_COMMAND_LEN 100

#define IRIDIUM_POWER_PIN 11
#define ROCKBLOCK_SLEEP_PIN 38
#define ROCKBLOCK_RING_PIN 40

typedef struct {

    long latitude;
    long longitude;

    long waypointLatitude;
    long waypointLongitude;

    short batteryVoltage;
    short batteryCurrent;

    // speed of prop between 0 and 255. Set to -1 for no change.
    short propSpeed;

} IridiumStatusMessage;


#define WAYPOINT_NO_CHANGE 0
#define WAYPOINT_OVERWRITE 1
#define WAYPOINT_APPEND 2

// Structure of messages received from Iridium.
typedef struct {

    // speed of prop between 0 and 255.
    short propSpeed;

    short bluetoothActive;

    // Time in seconds that status updates are sent through Iridium.
    long updateFrequencyMinutes;


    bool navigateToWaypoint;

    // Desired Waypoint
    // Actions
    //    Set to 0 for no change. (WAYPOINT_NO_CHANGE)
    //    Set to 1 delete existing waypoints and add this one (WAYPOINT_OVERWRITE)
    //    Set to 2 for append additional (WAYPOINT_APPEND)
    short waypointAction;

    short numWaypoints;
    Waypoint waypoints[10];

} IridiumCommandMessage;


class IridiumManager {

    private:
        bool iridiumActive;
        bool iridiumBusy;
        int signalQuality;

        // update frequency in minutes
        long updateFrequencyMinutes;
        unsigned long lastTransmitTime;

        // Serial3 on the Mega is pins TX3 pin 14 and RX3 pin 15
        // Iridium RXD pin1 -> RX3 pin 15
        // Iridium TXD pin6 -> TX3 pin 14
        HardwareSerial &IridiumSerial = Serial3;

        IridiumSBD modem{
          IridiumSerial,
          ROCKBLOCK_SLEEP_PIN,
          ROCKBLOCK_RING_PIN
        };

        int pollForIridiumRingAlerts();
        IridiumStatusMessage buildStatusMessage();
        int sendStatusReceiveCommandMessage(
            IridiumStatusMessage *txMessage,
            IridiumCommandMessage *rxCommandMessage
        );
        void processCommandMessage(
            IridiumCommandMessage *commandMessage
        );

        bool shouldTransmitStatus();
        //unsigned long getIridiumTime();


    public:
        void initialize();

        void wakeIridium();
        void sleepIridium();

        bool isIridiumActive();
        bool isIridiumBusy();
        unsigned long getLastTransmitTime();
        long getUpdateFrequency();

        void pollIridiumSignalQuality();
        int pollForCommandMessages(bool forceTransmitStatus);
};
#endif
