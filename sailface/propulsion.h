#ifndef _SAIL_FACE_PROPULSION
#define _SAIL_FACE_PROPULSION

#include <Arduino.h>
#include <stdint.h>
//# include "SCMD.h"
//# include "SCMD_config.h" //Contains #defines for common SCMD register names and values
#include "Wire.h"

#define PRIMARY_PROP_MOTOR 0
#define PRIMARY_PROP_PWM_PIN 13

class PropManager {

    private:
        int propSpeed;

    public:
        void initialize();
        void setPropellerSpeed(int newSpeed);
};
#endif
