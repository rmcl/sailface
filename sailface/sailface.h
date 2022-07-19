#ifndef _SAILFACE
#define _SAILFACE

#include <stdint.h>

void logDebugMessage(char c);
void logDebugMessage(int number);
void logDebugMessage(uint32_t number);
void logDebugMessage(float number);
void logDebugMessage(float number, int num_digits_after_decimal);
void logDebugMessage(long number);
void logDebugMessage(double number);
void logDebugMessage(char *message);

/* Define a data structure that is continuously written to the serial port
for ingestion into the main processor. */
typedef struct {
    // Position
    bool positionValid;

    // NOTE: lat/long are stored in units of decimal degrees*1x10e6
    // i.e. Multiply decimal degrees (in floating point representation) by
    // 1000000 to get an integer.

    // TODO: CONVERT LAT/LONG everywhere to longs from double to actually achieve
    // the above
    long latitude;
    long longitude;

    uint32_t time;     // UTC time
    uint32_t gpsFixAge;

    // actual speed & course from GPS
    double course; // course in degrees
    double speed; // speed in knots

    // MPU details
    float magneticCourse;
    float magneticCourseVariation;


    // Power Management
    float batteryVoltage;       // voltage in Volts out of the sunny buddy chargin unit.
    float batteryCurrentDraw;   // current in mA measured out of the sunny buddy charging unit

    // C&C state
    bool primaryPropulsionActive;
    bool bluetoothActive;
    bool iridiumActive;
    bool radioControlActive;
    short propSpeed;

    // the last time a status message
    // was sent relative to Millis() - the time the board started running
    // the current program

    // todo: this might not be a good idea! What if the battery is low and it grays
    //  out the microcontroller?
    // maybe replace with this: IridiumSBD::getSystemTime(struct tm &tm)
    uint32_t lastIridiumStatusMessageSentTime;
    uint32_t lastStatusMessageSentTime;

    // how frequently should we send updates via sattelite.
    // this should be persisted to EPROM in comms module.
    int iridiumUpdateFrequencySeconds;


    // Waypoint
    long waypointLatitude;
    long waypointLongitude;
    bool navigateToWaypoint;

    // Desired bearing to meet waypoint objective
    // Course is the direction from the previous waypoint to the next waypoint.
    // Bearing is the direction from the boat to the next waypoint.
    bool enablePIDControl;

    // Desired bearing towards waypoint.
    double desiredBearing;

    // Distance in meters between current location and waypoint.
    double distanceToWaypoint;

    // The signal quality accessing the iridium satellite network
    // This returns a number between 0 and 5. 2 or better is preferred.
    int iridiumSignalQuality;

} SailFaceStatus;

#endif
