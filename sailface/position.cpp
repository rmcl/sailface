#include "sailface.h"
#include "position.h"


void SailFacePositionManagement::initialize(SailFaceStatus *status) {
    gpsSerial.begin(9600);

    status->positionValid = false;
    status->latitude = 0;
    status->longitude = 0;
    status->gpsFixAge = -1;
    status->course = 0;
    status->speed = 0;
}

void SailFacePositionManagement::poll(SailFaceStatus *status) {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }

    if (gps.location.isValid()) {
        status->positionValid = gps.location.isValid();
        status->latitude = gps.location.lat();
        status->longitude = gps.location.lng();
        status->gpsFixAge = gps.location.age();
        status->course = gps.course.deg(); // course in degrees
        status->speed = gps.speed.knots(); // speed in knots
    }
}
