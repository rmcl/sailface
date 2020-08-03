#include "sailface.h"
#include "position.h"

/* When configuring accelerometer check out: https://www.i2cdevlib.com/forums/topic/8-mpu6050-connection-failed/ */


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

    //if (gps.location.isValid()) {
        status->positionValid = gps.location.isValid();
        status->latitude = gps.location.lat();
        status->longitude = gps.location.lng();
        status->gpsFixAge = gps.location.age();
        status->course = gps.course.deg(); // course in degrees
        status->speed = gps.speed.knots(); // speed in knots
    //}
}

void SailFacePositionManagement::writeStatusMessage(SailFaceStatus *status) {
    Serial.print("N:");
    Serial.print(status->positionValid);
    Serial.print(",");
    Serial.print(status->latitude, 6);
    Serial.print(",");
    Serial.print(status->longitude, 6);
    Serial.print(",");
    Serial.print(status->speed);
    Serial.print(",");
    Serial.print(status->course);
    Serial.print(",");
    Serial.print(status->gpsFixAge);
    Serial.print("\n");
}
