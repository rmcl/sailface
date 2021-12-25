#ifndef _SAIL_FACE_POWER
#define _SAIL_FACE_POWER

#include <Wire.h>
#include <Adafruit_INA219.h>

#include "sailface.h"



#define POWER_BATTERY_VOLTAGE_PIN A0
#define POWER_BATTERY_VOLTAGE_NUM_SAMPLES 5

#define BATTERY_CURRENT_MONITOR_ADDRESS 0x40
#define SOLAR_CURRENT_MONITOR_ADDRESS 0x41

class SailFacePowerManagement {

    private:
        Adafruit_INA219 batteryCurrentMonitor{BATTERY_CURRENT_MONITOR_ADDRESS};
        Adafruit_INA219 solarPanelCurrentMonitor{SOLAR_CURRENT_MONITOR_ADDRESS};

    public:
        void initialize(SailFaceStatus *status);
        void pollForBatteryStatus(SailFaceStatus *status);
        void writeStatusMessage(SailFaceStatus *status);
};
#endif
