/* Define a class for retrieving GPS position data. */
#include <time.h>
#include "sailface.h"
#include "position.h"


/* NOTE: When configuring accelerometer check out:
 * https://www.i2cdevlib.com/forums/topic/8-mpu6050-connection-failed/
 */

/*
 * Configure the position manager and initialize devices.
 */
void PositionManager::initialize() {

    pinMode(GPS_ENABLE_PIN, OUTPUT);
    digitalWrite(GPS_ENABLE_PIN, HIGH);

    pinMode(MPU_ENABLE_PIN, OUTPUT);
    digitalWrite(MPU_ENABLE_PIN, HIGH);

    gpsSerial.begin(9600);

    // This wire begin call is required for MPU setup.
    Wire.begin();
    delay(2000);

    if (!mpu.setup(0x68)) {
        if (bluetooth->isBluetoothActive()) {
            HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
            bluetoothDebug->println(
                String("ERR: MPU connection failed. Please check wiring")
            );
        }
    }

    /*
    mpu.setAccBias(1021.02, 46.71, 991.75);
    mpu.setGyroBias(-0.38, 0.81, -0.85);
    mpu.setMagBias(257.97, 315.69, -295.72);
    mpu.setMagScale(1.12, 0.92, 0.98);
    */

    // Load MPU calibration parameters from EPROM storage and configure the MPU
    // processing library.
    MPUCalibrationParams calibrationParams = persistedData->getMPUCalibrationParams();
    setMPUCalibrationParams(calibrationParams);

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
        if (millis() > prev_ms + 100) {

            float instantHeading = mpu.getYaw();
            /*
            float magX = mpu.getMagX();
            float magY = mpu.getMagY();

            if (magY > 0) {
                instantHeading = 90 - atan(magX/magY)*(180/M_PI);
            } else if (magY < 0) {
                instantHeading = 270 - atan(magX/magY)*(180/M_PI);
            } else if (magY == 0 && magX < 0) {
                instantHeading = 180;
            } else if (magY == 0 && magX > 0) {
                instantHeading = 0;
            }
            */

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

MPUCalibrationParams PositionManager::calibrateMPU(Stream *serial) {
    serial->println("Accel Gyro calibration will start in 5sec.");
    serial->println("Please leave the device still on the flat plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();

    serial->println("Mag calibration will start in 5sec.");
    serial->println("Please Wave device in a figure eight until done.");
    delay(5000);
    mpu.calibrateMag();

    MPUCalibrationParams calibrationParams;
    calibrationParams.accBiasX = mpu.getAccBiasX();
    calibrationParams.accBiasY = mpu.getAccBiasY();
    calibrationParams.accBiasZ = mpu.getAccBiasZ();

    calibrationParams.gyroBiasX = mpu.getGyroBiasX();
    calibrationParams.gyroBiasY = mpu.getGyroBiasY();
    calibrationParams.gyroBiasZ = mpu.getGyroBiasZ();

    calibrationParams.magBiasX = mpu.getMagBiasX();
    calibrationParams.magBiasY = mpu.getMagBiasY();
    calibrationParams.magBiasZ = mpu.getMagBiasZ();

    calibrationParams.magScaleX = mpu.getMagScaleX();
    calibrationParams.magScaleY = mpu.getMagScaleY();
    calibrationParams.magScaleZ = mpu.getMagScaleZ();

    mpu.verbose(false);

    persistedData->storeMPUCalibrationParams(calibrationParams);
    return calibrationParams;
}

void PositionManager::setMPUCalibrationParams(MPUCalibrationParams calibrationParams) {
    mpu.setAccBias(
        calibrationParams.accBiasX,
        calibrationParams.accBiasY,
        calibrationParams.accBiasZ);
    mpu.setGyroBias(
        calibrationParams.gyroBiasX,
        calibrationParams.gyroBiasY,
        calibrationParams.gyroBiasZ);
    mpu.setMagBias(
        calibrationParams.magBiasX,
        calibrationParams.magBiasY,
        calibrationParams.magBiasZ);
    mpu.setMagScale(
        calibrationParams.magScaleX,
        calibrationParams.magScaleY,
        calibrationParams.magScaleZ);
}

void PositionManager::printMPUCalibrationSettings(MPUCalibrationParams *params, Stream *serial) {
    serial->println("--Calibration Parameters---");
    serial->println("accel bias [g]: ");
    serial->print(params->accBiasX);// * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    serial->print(", ");
    serial->print(params->accBiasY);// * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    serial->print(", ");
    serial->print(params->accBiasZ);// * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    serial->println();
    serial->println("gyro bias [deg/s]: ");
    serial->print(params->gyroBiasX);// / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    serial->print(", ");
    serial->print(params->gyroBiasY);// / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    serial->print(", ");
    serial->print(params->gyroBiasZ);// / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    serial->println();
    serial->println("mag bias [mG]: ");
    serial->print(params->magBiasX);
    serial->print(", ");
    serial->print(params->magBiasY);
    serial->print(", ");
    serial->print(params->magBiasZ);
    serial->println();
    serial->println("mag scale []: ");
    serial->print(params->magScaleX);
    serial->print(", ");
    serial->print(params->magScaleY);
    serial->print(", ");
    serial->print(params->magScaleZ);
    serial->println();
    serial->println("--End Calibration Parameters---");
}