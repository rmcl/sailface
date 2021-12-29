#ifndef _SAIL_FACE_POSITION
#define _SAIL_FACE_POSITION

#include <Arduino.h>
#include <TinyGPS++.h>

#include "sailface.h"
#include "comms.h"

//#define GPS_RX_PIN 3
//#define GPS_TX_PIN 4

//serial ports: Serial1 on pins 19 (RX) and 18 (TX)
// #define gpsSerial Serial1


class SailFacePositionManagement {

    private:
        TinyGPSPlus gps;
        HardwareSerial &gpsSerial = Serial1;

        //https://www.i2cdevlib.com/forums/topic/8-mpu6050-connection-failed/
        //MPU6050 mpu{0x68};

    public:
        void initialize(SailFaceStatus *status);
        void pollGPSForPosition(SailFaceStatus *status);
        //void writeStatusMessage(SailFaceStatus *status);
        void writeStatusMessage(SailFaceCommunication *comms, SailFaceStatus *status);
};
#endif
