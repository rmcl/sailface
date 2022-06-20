#ifndef _SAIL_FACE_POSITION
#define _SAIL_FACE_POSITION

#include <Arduino.h>
#include <TinyGPS++.h>
#include "MPU9250.h"

#include "sailface.h"
#include "comms.h"


#define AVERAGE_COURSE_BUFFER_SIZE 50

class SailFacePositionManagement {

    private:
        TinyGPSPlus gps;

        //serial ports: Serial1 on pins 19 (RX) and 18 (TX)
        HardwareSerial &gpsSerial = Serial1;

        MPU9250 mpu;

        float averageCourseBuffer[AVERAGE_COURSE_BUFFER_SIZE];
        int averageCourseBufferIdx;

    public:
        void initialize(SailFaceStatus *status);
        void pollGPSForPosition(SailFaceStatus *status);
        void pollForMPU(SailFaceStatus *status);
        //void writeStatusMessage(SailFaceStatus *status);
        void writeStatusMessage(SailFaceCommunication *comms, SailFaceStatus *status);
};
#endif
