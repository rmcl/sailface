#ifndef _SAIL_FACE_HELM
#define _SAIL_FACE_HELM

#include <stdint.h>
#include <Arduino.h>
#include <Servo.h>

#include "sailface.h"

#define HELM_PWM_PIN 9
#define HELM_PWM_RANGE_MIN 0
#define HELM_PWM_RANGE_MAX 1000


class SailFaceHelm {

    private:
        Servo helmServo;
        uint32_t next;

    public:
        void initialize(SailFaceStatus *status);
        void poll(SailFaceStatus *status);

};
#endif
