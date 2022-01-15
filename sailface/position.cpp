/* Define a class for retrieving GPS position data. */
#include <time.h>
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

    //Serial.println("gps attempt");
    while (gpsSerial.available() > 0) {
        //Serial.println("gps available");
        gps.encode(gpsSerial.read());
    }

    status->positionValid = gps.location.isValid();
    status->latitude = gps.location.lat() * 1.0e6;
    status->longitude = gps.location.lng() * 1.0e6;
    status->gpsFixAge = gps.location.age();
    status->course = gps.course.deg(); // course in degrees
    status->speed = gps.speed.knots(); // speed in knots

    struct tm inputTime;
    inputTime.tm_year = gps.date.year();
    inputTime.tm_mon = gps.date.month();
    inputTime.tm_mday = gps.date.day();
    inputTime.tm_hour = gps.time.hour();
    inputTime.tm_min = gps.time.minute();
    inputTime.tm_sec = gps.time.second();
    inputTime.tm_isdst = -1;

    status->time = gmtime(mktime(&inputTime));

}

void SailFacePositionManagement::writeStatusMessage(SailFaceCommunication *comms, SailFaceStatus *status) {
    logDebugMessage("N:");
    logDebugMessage(status->positionValid);
    logDebugMessage(",");
    logDebugMessage(status->latitude,0);
    logDebugMessage(",");
    logDebugMessage(status->longitude,0);
    logDebugMessage(",");
    logDebugMessage(status->speed);
    logDebugMessage(",");
    logDebugMessage(status->course,1);
    logDebugMessage(",");
    logDebugMessage(status->gpsFixAge);
    logDebugMessage("\n");
}
