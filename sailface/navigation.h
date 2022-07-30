/* Implementation methods to compute course. Store waypoints in EEPROM */
#ifndef _SAIL_FACE_NAVIGATION
#define _SAIL_FACE_NAVIGATION

#include <Arduino.h>
#include <stdint.h>
#include <math.h>
#include <EEPROM.h>
#include <TinyGPS++.h>

// determine if these are defined elsewhere
#define RAD2DEG M_PI/360.0*2.0
#define DEG2RAD 360.0/M_PI/2.0

#define SAILFACE_EEPROM_ADDRESS 0x0

typedef struct {
    long latitude;
    long longitude;
} Waypoint;

typedef struct {
    // Distance in meters between current location and waypoint.
    double distanceToWaypoint;
    double bearing;
} Bearing;

//
// Store waypoint goal lat/long in EEPROM so it is persisted across power cycles.
// Use GPS data and operator input to decide course.
//
class NavigationManager {

    private:
        Waypoint currentWaypoint;
        bool navigateToWaypoint;

    public:
        void initialize();
        long computeBearingToNextWaypoint(
            long curLatitude,
            long curLongitude
        );
        void setWaypoint(long latitude, long longitude);
        Waypoint getNextWaypoint();
        bool isNavigatingToWaypoint();

};
#endif
