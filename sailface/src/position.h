#ifndef _SAIL_FACE_POSITION
#define _SAIL_FACE_POSITION

#include <Arduino.h>
#include <TinyGPS++.h>
#include "MPU9250.h"



#define AVERAGE_HEADING_BUFFER_SIZE 5

// PIN must be set to HIGH for GPS to be powered
// GPS FUSED ON IN V0.2
#define GPS_ENABLE_PIN 8
#define MPU_ENABLE_PIN 10

/* Define a data structure stores position and heading information */
typedef struct {
    // Position
    bool positionValid;

    // NOTE: lat/long are stored in units of decimal degrees*1x10e6
    // i.e. Multiply decimal degrees (in floating point representation) by
    // 1000000 to get an integer.

    // TODO: CONVERT LAT/LONG everywhere to longs from double to actually achieve
    // the above
    long latitude;
    long longitude;

    uint32_t time;     // UTC time
    uint32_t gpsFixAge;

    // actual speed & course from GPS
    double course; // course in degrees
    double speed; // speed in knots

    // MPU details
    float magneticHeading;
    float magneticHeadingVariation;

} PositionInfo;


typedef struct {
    float accBiasX;
    float accBiasY;
    float accBiasZ;

    float gyroBiasX;
    float gyroBiasY;
    float gyroBiasZ;

    float magBiasX;
    float magBiasY;
    float magBiasZ;

    float magScaleX;
    float magScaleY;
    float magScaleZ;

} MPUCalibrationParams;


class PositionManager {

    private:
        TinyGPSPlus gps;

        //serial ports: Serial1 on pins 19 (RX) and 18 (TX)
        Stream &gpsSerial = Serial1;

        MPU9250 mpu;
        uint32_t prev_ms = 0;

        PositionInfo currentPosition;

        float averageHeadingBuffer[AVERAGE_HEADING_BUFFER_SIZE];
        int averageHeadingBufferIdx;

        void setMPUCalibrationParams(MPUCalibrationParams calibrationParams);

    public:
        void initialize();
        void pollGPSForPosition();
        void pollForMPU();

        MPUCalibrationParams calibrateMPU(Stream *serial);
        void printMPUCalibrationSettings(MPUCalibrationParams *params, Stream *serial);

        PositionInfo getCurPosition();
};
#endif
