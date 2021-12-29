/* Implementation methods to compute course. Store waypoints in EEPROM */
#include "sailface.h"
#include "navigation.h"


//
// Initialize the navigation module. Retrieve stored waypoint and other data
// from the EEPROM persistant storage.
//
void SailFaceNavigation::initialize(SailFaceStatus *status) {
    SailFacePersistedData persistedData;
    EEPROM.get(SAILFACE_EEPROM_ADDRESS, persistedData);

    status->waypointLatitude = persistedData.waypointLatitude;
    status->waypointLongitude = persistedData.waypointLongitude;

    Serial.print("NAV DATA:");
    Serial.print(persistedData.waypointLatitude);
    Serial.print(",");
    Serial.print(persistedData.waypointLongitude);
}

//
// Use the current lat/long in SF Status as well as desired waypoint to
// compute a course that SF should traverse.
//
void SailFaceNavigation::recomputeCourseToWaypoint(SailFaceStatus *status) {
    if (!status->positionValid) {
        Serial.println("Course cannot be computed. GPS Position invalid.");
        return;
    }

    /*
    float newCourse = course_to(
        status->latitude,
        status->longitude,
        status->waypointLatitude,
        status->waypointLongitude,
        &status->distanceToWaypoint);
    */

    Serial.println("plotting a course");
    status->desiredCourse = -1;
}

//
// Update the lat/long which sailface should head towards.
//
void SailFaceNavigation::setWaypoint(double latitude, double longitude) {
    SailFacePersistedData persistedData = {
        latitude,
        longitude
    };

    EEPROM.put(SAILFACE_EEPROM_ADDRESS, persistedData);
}


// find the bearing and distance in meters from point 1 to 2,
// using the equirectangular approximation
// lat and lon are degrees*1.0e6, 10 cm precision
// Source: https://forum.arduino.cc/t/arduino-haversine-program/379208/14
float SailFaceNavigation::course_to(long lat1, long lon1, long lat2, long lon2, float* distance) {

	float dlam,dphi,radius= 6371000.0;

	dphi = DEG2RAD*(lat1+lat2)*0.5e-6; //average latitude in radians
	float cphi=cos(dphi);

	dphi = DEG2RAD*(lat2-lat1)*1.0e-6; //differences in degrees (to radians)
	dlam = DEG2RAD*(lon2-lon1)*1.0e-6;

	dlam *= cphi;  //correct for latitude

	float bearing=RAD2DEG*atan2(dlam,dphi);
	if(bearing<0) bearing=bearing+360.0;

	*distance = radius * sqrt(dphi*dphi + dlam*dlam);
	return bearing;
}
