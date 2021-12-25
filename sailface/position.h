#ifndef _SAIL_FACE_POSITION
#define _SAIL_FACE_POSITION

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#include "sailface.h"
#include "comms.h"

#define GPS_RX_PIN 3
#define GPS_TX_PIN 4


class SailFacePositionManagement {

    private:
        TinyGPSPlus gps;
        SoftwareSerial gpsSerial{GPS_RX_PIN, GPS_TX_PIN};

    public:
        void initialize(SailFaceStatus *status);
        void pollGPSForPosition(SailFaceStatus *status);
        //void writeStatusMessage(SailFaceStatus *status);
        void writeStatusMessage(SailFaceCommunication *comms, SailFaceStatus *status);
};
#endif
