/* Implementation methods to compute course. Store waypoints in EEPROM */
#include "sailface.h"
#include "navigation.h"


//
// Initialize the navigation module. Retrieve stored waypoint and other data
// from the EEPROM persistant storage.
//
void NavigationManager::initialize() {

    Waypoint *waypoints;
    numWaypoints = persistedData->getWaypoints(waypoints);

    if (numWaypoints > 0) {
        activeWaypoint = waypoints[0];

        navigateToWaypoint = persistedData->getIsNavigatingToWaypoint();
    } else {
        // If no waypoints are defined then we shouldn't try to navigate to them
        // even if the EEPROM store says that we have enabled navigation.
        navigateToWaypoint = false;
    }
}

bool NavigationManager::startNavigatingToWaypoint() {
    if (navigateToWaypoint == false) {
        if (numWaypoints > 0) {
            navigateToWaypoint = true;
            return true;
        }
    }
    return false;
}

void NavigationManager::stopNavigatingToWaypoint() {
    navigateToWaypoint = false;
}

bool NavigationManager::isNavigatingToWaypoint() {
    return navigateToWaypoint;
}

//
// Use the current lat/long in SF Status as well as desired waypoint to
// compute a course that SF should traverse.
//
long NavigationManager::computeBearingToActiveWaypoint(
    long curLatitude,
    long curLongitude
) {

    if (!navigateToWaypoint) {
        return -366;
    }

    return TinyGPSPlus::courseTo(
        curLatitude * 1e-6,
        curLongitude * 1e-6,
        activeWaypoint.latitude * 1e-6,
        activeWaypoint.longitude * 1e-6);
}

// Return the distance from the given lat/long to the active waypoint in
// METERS
long NavigationManager::computeDistanceToActiveWaypoint(
    long curLatitude,
    long curLongitude
) {

    if (!navigateToWaypoint) {
        return LONG_MAX;
    }

    long distanceKm = TinyGPSPlus::distanceBetween(
        curLatitude * 1e-6,
        curLongitude * 1e-6,
        activeWaypoint.latitude * 1e-6,
        activeWaypoint.longitude * 1e-6);

    // Convert to meters
    return distanceKm * 1000;
}

//
// Update the lat/long which sailface should head towards.
//
void NavigationManager::updateWaypoints(Waypoint *waypoints, short numWaypoints, bool appendWaypoints) {
    if (appendWaypoints == false) {
        if (numWaypoints == 0) {
            activeWaypoint = {0};
            navigateToWaypoint = false;
        } else {
            activeWaypoint = waypoints[0];
            navigateToWaypoint = true;
        }
    }

    persistedData->storeWaypoints(waypoints, numWaypoints, appendWaypoints);

}
/*
void NavigationManager::setWaypoint(long latitude, long longitude) {
    activeWaypoint.latitude = latitude;
    activeWaypoint.longitude = longitude;
    navigateToWaypoint = true;

    persistedData->storeWaypoints(&activeWaypoint, 1);
}
*/

Waypoint NavigationManager::getActiveWaypoint() {
    return activeWaypoint;
}

bool NavigationManager::hasAchievedActiveWaypoint(
    long curLatitude,
    long curLongitude
) {
    long distanceMeters = computeDistanceToActiveWaypoint(curLatitude, curLongitude);
    return distanceMeters < activeWaypoint.allowedRadiusMeters;
}

//
// Advance to the next waypoint if defined
// if there are no more waypoints to navigate to then return false, else return true.
//
bool NavigationManager::advanceToNextWaypoint() {
    if (numWaypoints > 1) {
        // There are additional waypoints to navigate to.
        Waypoint *waypoints;
        persistedData->getWaypoints(waypoints);
        activeWaypoint = waypoints[1];

        numWaypoints--;

        persistedData->storeWaypoints(&(waypoints[1]), numWaypoints, true);
    } else {
        // No more waypoints. Clear the current waypoint and set numWaypoints to false.
        activeWaypoint = {0};
        numWaypoints = 0;

        // Clear out the old stored waypoint.
        persistedData->storeWaypoints(NULL, 0, true);
    }

    // Return true if there are additional waypoints to navigate to.
    return numWaypoints > 0;
}

void NavigationManager::pollForNavigationAdjustments() {
    PositionInfo curPosition = position->getCurPosition();
    if (curPosition.positionValid == false) {
        // Nothing to do until we acquire our position using GPS.
        return;
    }

    // Determine if we have reached the current active waypoint.
    if (isNavigatingToWaypoint()) {
        bool hasAchieved = hasAchievedActiveWaypoint(
            curPosition.latitude,
            curPosition.longitude
        );

        if (hasAchieved) {
            bool hasAdditionalWaypoints = advanceToNextWaypoint();
            if (hasAdditionalWaypoints == false) {

                // If no additional waypoints they stop the primary prop.
                prop->setPropellerSpeed(0);
            }

            // Force the transmission of a status message to notify that
            // we have achieved the waypoint
            iridium->pollForCommandMessages(true);
        }
    }

    // Check again to see if we are still navigating to a waypoint.
    // If we just achieved a waypoint and don't have any further objectives
    // then navigation should seace.
    if (isNavigatingToWaypoint()) {
        double bearing = computeBearingToActiveWaypoint(
            curPosition.latitude,
            curPosition.longitude
        );

        /// figure out a more appopriate error state
        helm->pollForRudderAdjustment(
            curPosition.magneticHeading,
            curPosition.magneticHeadingVariation,
            bearing
        );
    }
}
