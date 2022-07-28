/* Define a class for retrieving GPS position data. */
#include <time.h>
#include "sailface.h"
#include "position.h"
#include "comms.h"


/* When configuring accelerometer check out: https://www.i2cdevlib.com/forums/topic/8-mpu6050-connection-failed/ */

/*
 * Configure the position manager and initialize devices.
 */
void PositionManager::initialize() {

    pinMode(GPS_ENABLE_PIN, OUTPUT);
    digitalWrite(GPS_ENABLE_PIN, HIGH);

    pinMode(MPU_ENABLE_PIN, OUTPUT);
    digitalWrite(MPU_ENABLE_PIN, HIGH);

    gpsSerial.begin(9600);

    if (!mpu.setup(0x68)) {
        Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
    }

    mpu.setMagBias(171.38, 633.19, -22.61);
    mpu.setMagScale(1.75, 0.47, 3.22);

    averageHeadingBufferIdx = 0;
    for ( int n = 0; n < AVERAGE_HEADING_BUFFER_SIZE; ++n ) {
        averageHeadingBuffer[n] = 0;
    }

    currentPosition.positionValid = false;
    currentPosition.latitude = 0;
    currentPosition.longitude = 0;
    currentPosition.gpsFixAge = 0;
    currentPosition.course = 0;
    currentPosition.speed = 0;

    currentPosition.magneticHeading = 0;
    currentPosition.magneticHeadingVariation = 0;
}

void PositionManager::pollForMPU() {

    if (mpu.update()) {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 100) {

            float magX = mpu.getMagX();
            float magY = mpu.getMagY();
            float instantHeading;
            if (magY > 0) {
                instantHeading = 90 - atan(magX/magY)*(180/M_PI);
            } else if (magY < 0) {
                instantHeading = 270 - atan(magX/magY)*(180/M_PI);
            } else if (magY == 0 && magX < 0) {
                instantHeading = 180;
            } else if (magY == 0 && magX > 0) {
                instantHeading = 0;
            }

            averageHeadingBuffer[averageHeadingBufferIdx] = instantHeading;
            averageHeadingBufferIdx = (averageHeadingBufferIdx + 1) % AVERAGE_HEADING_BUFFER_SIZE;

            float sum = 0;
            for ( int idx = 0; idx < AVERAGE_HEADING_BUFFER_SIZE; ++idx ) {
                sum += averageHeadingBuffer[idx];
            }
            currentPosition.magneticHeading = sum / AVERAGE_HEADING_BUFFER_SIZE;

            prev_ms = millis();
        }
    }
}
void PositionManager::pollGPSForPosition() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }

    currentPosition.positionValid = gps.location.isValid();
    currentPosition.latitude = gps.location.lat() * 1.0e6;
    currentPosition.longitude = gps.location.lng() * 1.0e6;
    currentPosition.gpsFixAge = gps.location.age();
    currentPosition.course = gps.course.deg(); // course in degrees
    currentPosition.speed = gps.speed.knots(); // speed in knots

    struct tm inputTime;
    inputTime.tm_year = gps.date.year();
    inputTime.tm_mon = gps.date.month();
    inputTime.tm_mday = gps.date.day();
    inputTime.tm_hour = gps.time.hour();
    inputTime.tm_min = gps.time.minute();
    inputTime.tm_sec = gps.time.second();
    inputTime.tm_isdst = -1;

    currentPosition.time = gmtime(mktime(&inputTime));
}

PositionInfo PositionManager::getCurPosition() {
    return currentPosition;
}
