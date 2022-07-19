/* Define a class for retrieving GPS position data. */
#include <time.h>
#include "sailface.h"
#include "position.h"
#include "comms.h"


/* When configuring accelerometer check out: https://www.i2cdevlib.com/forums/topic/8-mpu6050-connection-failed/ */


void SailFacePositionManagement::initialize(SailFaceStatus *status) {

    pinMode(GPS_ENABLE_PIN, OUTPUT);
    digitalWrite(GPS_ENABLE_PIN, HIGH);

    pinMode(MPU_ENABLE_PIN, OUTPUT);
    digitalWrite(MPU_ENABLE_PIN, HIGH);

    delay(1000);

    gpsSerial.begin(9600);

    if (!mpu.setup(0x68)) {
        Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
    }

    mpu.setMagBias(171.38, 633.19, -22.61);
    mpu.setMagScale(1.75, 0.47, 3.22);

    averageCourseBufferIdx = 0;
    for ( int n = 0; n < AVERAGE_COURSE_BUFFER_SIZE; ++n ) {
        averageCourseBuffer[n] = 0;
    }

    status->positionValid = false;
    status->latitude = 0;
    status->longitude = 0;
    status->gpsFixAge = 0;
    status->course = 0;
    status->speed = 0;

    status->magneticCourse = 0;
    status->magneticCourseVariation = 0;
}

void SailFacePositionManagement::pollForMPU(SailFaceStatus *status) {

    if (mpu.update()) {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 100) {

            float magX = mpu.getMagX();
            float magY = mpu.getMagY();
            float heading;
            if (magY > 0) {
                heading = 90 - atan(magX/magY)*(180/M_PI);
            } else if (magY < 0) {
                heading = 270 - atan(magX/magY)*(180/M_PI);
            } else if (magY == 0 && magX < 0) {
                heading = 180;
            } else if (magY == 0 && magX > 0) {
                heading = 0;
            }

            averageCourseBuffer[averageCourseBufferIdx] = heading;
            averageCourseBufferIdx = (averageCourseBufferIdx + 1) % AVERAGE_COURSE_BUFFER_SIZE;

            float sum = 0;
            for ( int idx = 0; idx < AVERAGE_COURSE_BUFFER_SIZE; ++idx ) {
                sum += averageCourseBuffer[idx];
            }
            status->magneticCourse = sum / AVERAGE_COURSE_BUFFER_SIZE;

            prev_ms = millis();
        }
    }

    // Serial.print("Mag X, Y, Z, angle: ");
    // Serial.print(mpu.getMagX(), 2);
    // Serial.print(", ");
    // Serial.print(mpu.getMagY(), 2);
    // Serial.print(", ");
    // Serial.print(mpu.getMagZ(), 2);
    // Serial.print(", ");

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
    logDebugMessage("GPS:");
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
    logDebugMessage("MPU:");
    logDebugMessage(status->magneticCourse);
    logDebugMessage(",");
    logDebugMessage(status->magneticCourseVariation);
    logDebugMessage("\n");
}
