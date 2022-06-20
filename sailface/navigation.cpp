/* Implementation methods to compute course. Store waypoints in EEPROM */
#include "sailface.h"
#include "navigation.h"

#include <TinyGPS++.h>


//
// Initialize the navigation module. Retrieve stored waypoint and other data
// from the EEPROM persistant storage.
//
void SailFaceNavigation::initialize(SailFaceStatus *status) {
    SailFacePersistedData persistedData;
    EEPROM.get(SAILFACE_EEPROM_ADDRESS, persistedData);

    status->waypointLatitude = persistedData.waypointLatitude;
    status->waypointLongitude = persistedData.waypointLongitude;

    logDebugMessage("PERSISTED NAV DATA:");
    logDebugMessage(persistedData.waypointLatitude, 0);
    logDebugMessage(",");
    logDebugMessage(persistedData.waypointLongitude, 0);
    logDebugMessage("\n");
}

//
// Use the current lat/long in SF Status as well as desired waypoint to
// compute a course that SF should traverse.
//
void SailFaceNavigation::recomputeCourseToWaypoint(SailFaceStatus *status) {
    if (!status->positionValid) {
        //logDebugMessage("Course cannot be computed. GPS Position invalid.\n");
        return;
    }

    if (!status->navigateToWaypoint) {
        return;
    }

    status->desiredBearing = TinyGPSPlus::courseTo(
        status->latitude * 1e-6,
        status->longitude * 1e-6,
        status->waypointLatitude * 1e-6,
        status->waypointLongitude * 1e-6);

    status->distanceToWaypoint = TinyGPSPlus::distanceBetween(
        status->latitude * 1e-6,
        status->longitude * 1e-6,
        status->waypointLatitude * 1e-6,
        status->waypointLongitude * 1e-6);

}

//
// Update the lat/long which sailface should head towards.
//
void SailFaceNavigation::setWaypoint(long latitude, long longitude, SailFaceStatus *status) {
    SailFacePersistedData persistedData = {
        latitude,
        longitude
    };

    EEPROM.put(SAILFACE_EEPROM_ADDRESS, persistedData);

    status->waypointLatitude = latitude;
    status->waypointLongitude = longitude;
}
