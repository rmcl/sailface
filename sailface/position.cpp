#include "sailface.h"
#include "position.h"
#include "comms.h"

/* When configuring accelerometer check out: https://www.i2cdevlib.com/forums/topic/8-mpu6050-connection-failed/ */


void SailFacePositionManagement::initialize(SailFaceStatus *status) {
    gpsSerial.begin(9600);

    status->positionValid = false;
    status->latitude = 0;
    status->longitude = 0;
    status->gpsFixAge = 0;
    status->course = 0;
    status->speed = 0;
}

void SailFacePositionManagement::pollGPSForPosition(SailFaceStatus *status) {


    //switch to listening on this software serial
    if (!gpsSerial.isListening()) {
        Serial.println("gps is not listening");
        gpsSerial.listen();
    } else {
        Serial.println("gps is listening");
    }

    Serial.println("gps attempt");
    while (gpsSerial.available() > 0) {
        //Serial.println("gps available");
        //Serial.write(gpsSerial.read());
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

void SailFacePositionManagement::writeStatusMessage(SailFaceCommunication *comms, SailFaceStatus *status) {
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
