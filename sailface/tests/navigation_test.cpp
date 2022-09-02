#include <AUnit.h>

#include "../navigation.h"

test(SetAndGetWaypoint) {
    NavigationManager navigationManager;

    Waypoint waypoints[1];
    waypoints[0].longitude = 123.22;
    waypoints[0].latitude = 456.67;
    waypoints[0].allowedRadiusMeters = 100;

    navigationManager.updateWaypoints(waypoints, 1, true);
    Waypoint wp = navigationManager.getActiveWaypoint();

    /*
    assertEqual(wp.latitude, 123.22);
    assertEqual(wp.longitude, 456.67);*/
}
