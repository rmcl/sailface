#ifndef _SAIL_FACE_COMMS
#define _SAIL_FACE_COMMS

#include <stdint.h>
#include <Arduino.h>

#include "sailface.h"

#define HELM_PWM_PIN 9
#define HELM_PWM_RANGE_MIN 0
#define HELM_PWM_RANGE_MAX 1000


class SailFaceCommunication {

    private:


    public:
        void initialize(SailFaceStatus *status);
        void poll(SailFaceStatus *status);

};
#endif
