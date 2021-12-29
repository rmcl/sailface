/* Implementation methods to compute course. Store waypoints in EEPROM */
#ifndef _SAIL_FACE_NAVIGATION
#define _SAIL_FACE_NAVIGATION

#include <Arduino.h>
#include <stdint.h>
#include <math.h>
#include <EEPROM.h>

#include "sailface.h"

// determine if these are defined elsewhere
#define RAD2DEG M_PI/360.0*2.0
#define DEG2RAD 360.0/M_PI/2.0


#define SAILFACE_EEPROM_ADDRESS 0x0

typedef struct {
    double waypointLatitude;
    double waypointLongitude;

} SailFacePersistedData;

//
// Store waypoint goal lat/long in EEPROM so it is persisted across power cycles.
// Use GPS data and operator input to decide course.
//
class SailFaceNavigation {

    private:
        float course_to(long lat1, long lon1, long lat2, long lon2, float* distance);

    public:
        void initialize(SailFaceStatus *status);
        void recomputeCourseToWaypoint(SailFaceStatus *status);
        void setWaypoint(double latitude, double longitude);

};
#endif
