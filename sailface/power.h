#ifndef _SAIL_FACE_POWER
#define _SAIL_FACE_POWER

#include <Wire.h>
#include <Adafruit_INA219.h>


#define POWER_BATTERY_VOLTAGE_PIN A0
#define POWER_BATTERY_VOLTAGE_NUM_SAMPLES 5

// # define BATTERY_CURRENT_MONITOR_ADDRESS 0x40

//Details of the power related systems.
typedef struct {
    float batteryVoltage;       // voltage in Volts out of the sunny buddy chargin unit.
    float batteryCurrentDraw;   // current in mA measured out of the sunny buddy charging unit

} PowerInfo;

class PowerManager {

    private:
        Adafruit_INA219 batteryCurrentMonitor; //{BATTERY_CURRENT_MONITOR_ADDRESS};

    public:
        void initialize();
        void getPowerInfo(PowerInfo *info);
};
#endif
