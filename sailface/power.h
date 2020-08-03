#ifndef _SAIL_FACE_POWER
#define _SAIL_FACE_POWER

#include <Wire.h>
#include <Adafruit_INA219.h>

#include "sailface.h"



#define POWER_BATTERY_VOLTAGE_PIN A0
#define POWER_BATTERY_VOLTAGE_NUM_SAMPLES 5


class SailFacePowerManagement {

    private:
        Adafruit_INA219 batteryCurrentMonitor;

    public:
        void initialize(SailFaceStatus *status);
        void poll(SailFaceStatus *status);
        void writeStatusMessage(SailFaceStatus *status);
};
#endif
