/* Implementation methods to compute course. Store waypoints in EEPROM */
#include "navigation.h"


//
// Initialize the navigation module. Retrieve stored waypoint and other data
// from the EEPROM persistant storage.
//
void NavigationManager::initialize() {
    EEPROM.get(SAILFACE_EEPROM_ADDRESS, currentWaypoint);

    // TODO: Not sure if this is a good assumption
    navigateToWaypoint = false;
}

bool NavigationManager::isNavigatingToWaypoint() {
    return navigateToWaypoint;
}

//
// Use the current lat/long in SF Status as well as desired waypoint to
// compute a course that SF should traverse.
//
long NavigationManager::computeBearingToNextWaypoint(
    long curLatitude,
    long curLongitude
) {

    if (!navigateToWaypoint) {
        return -366;
    }

    return TinyGPSPlus::courseTo(
        curLatitude * 1e-6,
        curLongitude * 1e-6,
        currentWaypoint.latitude * 1e-6,
        currentWaypoint.longitude * 1e-6);

    /*
    newBearing->distanceToWaypoint = TinyGPSPlus::distanceBetween(
        curLatitude * 1e-6,
        curLongitude * 1e-6,
        currentWaypoint.latitude * 1e-6,
        currentWaypoint.longitude * 1e-6);
    */
}

//
// Update the lat/long which sailface should head towards.
//
void NavigationManager::setWaypoint(long latitude, long longitude) {
    currentWaypoint.latitude = latitude;
    currentWaypoint.longitude = longitude;
    navigateToWaypoint = true;
    EEPROM.put(SAILFACE_EEPROM_ADDRESS, currentWaypoint);

}
