/* Implementation methods to compute course. Store waypoints in EEPROM */
#ifndef _SAIL_FACE_NAVIGATION
#define _SAIL_FACE_NAVIGATION

// Only import these things if we are building for the device
// and not compiling to run unit tests.
#ifdef ARDUINO_VERSION
    #include "Arduino.h"
    #include <EEPROM.h>
#endif

#include <stdint.h>
#include <math.h>
#include <TinyGPS++.h>

// determine if these are defined elsewhere
#define RAD2DEG M_PI/360.0*2.0
#define DEG2RAD 360.0/M_PI/2.0

#define SF_EEPROM_WAYPOINT_ADDRESS 0x0


typedef struct {
    long latitude;
    long longitude;

    // The radius in meters defining the circle centered on the waypoint
    // where the navigation subsystem will accept the waypoint as being reached.
    long allowedRadiusMeters;

} Waypoint;


//
// Store waypoint goal lat/long in EEPROM so it is persisted across power cycles.
// Use GPS data and operator input to decide course.
//
class NavigationManager {

    private:
        int numWaypoints;
        Waypoint activeWaypoint;
        bool navigateToWaypoint;

    public:
        void initialize();
        long computeBearingToActiveWaypoint(
            long curLatitude,
            long curLongitude
        );
        long computeDistanceToActiveWaypoint(
            long curLatitude,
            long curLongitude
        );

        void updateWaypoints(Waypoint *waypoints, short numWaypoints, bool appendWaypoints);
        Waypoint getActiveWaypoint();

        bool isNavigatingToWaypoint();
        bool startNavigatingToWaypoint();
        void stopNavigatingToWaypoint();

        bool hasAchievedActiveWaypoint(
            long curLatitude,
            long curLongitude
        );

        bool advanceToNextWaypoint();

        void pollForNavigationAdjustments();
};
#endif
