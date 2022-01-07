/* Adjust the rudder to keep SailFace on a desired bearing.

Utilize proportional, integral, derivative (PID) theory to control the Rudder
based on desired bearing and current heading.

*/
#ifndef _SAIL_FACE_HELM
#define _SAIL_FACE_HELM

#include <stdint.h>
#include <Arduino.h>
#include <Servo.h>
#include <PID_v1.h>

#include "sailface.h"



// 155 - All the way to PORT
// 120 - Centered
// 85 - All the way to STARBOARD
#define RUDDER_SERVO_MAX_PORT 155
#define RUDDER_SERVO_MAX_STARBOARD 85
#define RUDDER_SERVO_CENTER 120

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
        PID *rudderPID;

        double pidRudderPositionOut = 0;
        double difference = 1;
        int Kp = 3;
        int Ki = 2;
        int Kd = 1;


    public:
        void initialize(SailFaceStatus *status);

        // The angle that we want the boat to go in
        void setBearingAndEnablePID(int bearing, SailFaceStatus *status);
        void disablePID(SailFaceStatus *status);
        void setRudderPosition(int position);

        // Adjust the rudder to keep us on the desired bearing.
        void pollForRudderAdjustment(SailFaceStatus *status);

};
#endif
