#include <Wire.h>

class Adafruit_INA219 {
    public:
        Adafruit_INA219(uint8_t addr = 0x48);
        ~Adafruit_INA219();
        bool begin(TwoWire *theWire = &Wire);
        void setCalibration_32V_2A();
        void setCalibration_32V_1A();
        void setCalibration_16V_400mA();
        float getBusVoltage_V();
        float getShuntVoltage_mV();
        float getCurrent_mA();
        float getPower_mW();
        void powerSave(bool on);
        bool success();

    private:
        Adafruit_I2CDevice *i2c_dev = NULL;

        bool _success;

        uint8_t ina219_i2caddr = -1;
        uint32_t ina219_calValue;
        // The following multipliers are used to convert raw current and power
        // values to mA and mW, taking into account the current config settings
        uint32_t ina219_currentDivider_mA;
        float ina219_powerMultiplier_mW;

        void init();
        int16_t getBusVoltage_raw();
        int16_t getShuntVoltage_raw();
        int16_t getCurrent_raw();
        int16_t getPower_raw();
};
