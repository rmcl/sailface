#ifndef _SAIL_FACE_HELM
#define _SAIL_FACE_HELM

#include <stdint.h>
#include <Arduino.h>
#include <Servo.h>

#include "sailface.h"

#define HELM_PWM_PIN 5

/*
Servo is a SF4328MG
http://wiki.sunfounder.cc/images/9/9a/TD-8120MG_Digital_Servo.pdf
*/
#define HELM_PWM_RANGE_MIN 500
#define HELM_PWM_RANGE_MAX 2500


class SailFaceHelm {

    private:
        Servo helmServo;

    public:
        void initialize(SailFaceStatus *status);
        void setRudderPosition(int position);

};
#endif
