#ifndef _SAIL_FACE_PROPULSION
#define _SAIL_FACE_PROPULSION

#include <Arduino.h>
#include <stdint.h>
#include "SCMD.h"
#include "SCMD_config.h" //Contains #defines for common SCMD register names and values
#include "Wire.h"

#define PRIMARY_PROP_MOTOR 0

class SailFacePropulsion {

    private:
        SCMD motorDriver;

    public:
        void initialize(SailFaceStatus *status);
        void setPropellerSpeed(uint32_t speed, SailFaceStatus *status);
};
#endif
