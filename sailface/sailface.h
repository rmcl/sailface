#ifndef _SAILFACE
#define _SAILFACE

#include <stdint.h>

void logDebugMessage(char *message);

/* Define a data structure that is continuously written to the serial port
for ingestion into the main processor. */
typedef struct {
    // Position
    bool positionValid;

    // NOTE: lat/long are stored in units of decimal degrees*1x106
    // i.e. Multiply decimal degrees (in floating point representation) by
    // 1000000 to get an integer.

    // TODO: CONVERT LAT/LONG everywhere to longs from double to actually achieve
    // the above
    double latitude;
    double longitude;

    uint32_t time;     // UTC time
    uint32_t gpsFixAge;

    // actual speed & course from GPS
    double course; // course in degrees
    double speed; // speed in knots

    // Power Management
    float batteryVoltage;       // voltage in Volts out of the sunny buddy chargin unit.
    float solarPanelCurrent;    // current in mA measured in series with the solar panels
    float batteryCurrentDraw;   // current in mA measured out of the sunny buddy charging unit

    // C&C state
    bool primaryPropulsionActive;
    bool bluetoothActive;
    bool iridiumActive;
    int propSpeed;

    // the last time a status message
    // was sent relative to Millis() - the time the board started running
    // the current program

    // todo: this might not be a good idea! What if the battery is low and it grays
    //  out the microcontroller?
    // maybe replace with this: IridiumSBD::getSystemTime(struct tm &tm)
    unsigned long lastStatusMessageTime;


    // Waypoint
    double waypointLatitude;
    double waypointLongitude;

    // Desired course to meet waypoint objective
    double desiredCourse;
    float distanceToWaypoint;

    // The signal quality accessing the iridium satellite network
    // This returns a number between 0 and 5. 2 or better is preferred.
    int iridiumSignalQuality;

} SailFaceStatus;

#endif
