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



// 155 - All the way to PORT
// 120 - Centered
// 85 - All the way to STARBOARD
#define RUDDER_SERVO_MAX_PORT 155
#define RUDDER_SERVO_MAX_STARBOARD 85
#define RUDDER_SERVO_CENTER 120

//used to be 5 now i think its
#define HELM_PWM_PIN 3

/*
Servo is a SF4328MG
http://wiki.sunfounder.cc/images/9/9a/TD-8120MG_Digital_Servo.pdf
*/
#define HELM_PWM_RANGE_MIN 500
#define HELM_PWM_RANGE_MAX 2500


class HelmManager {

    private:
        Servo helmServo;
        PID *rudderPID;

        // Desired bearing to meet waypoint objective
        // Course is the direction from the previous waypoint to the next waypoint.
        // Bearing is the direction from the boat to the next waypoint.
        bool enablePIDControl;

        // Desired bearing towards waypoint.
        double desiredBearing;

        double pidRudderPositionOut = 0;
        double difference = 0;
        double setPoint = 0;
        int Kp = 3;
        int Ki = 2;
        int Kd = 1;

        long lastAdjustTime = 0;


    public:
        void initialize();

        // The angle that we want the boat to go in
        void setBearingAndEnablePID(int bearing);
        void disablePID();
        void setRudderPosition(int position);

        // Adjust the rudder to keep us on the desired bearing.
        void pollForRudderAdjustment(
            double curMagHeading,
            double curMagHeadingVariation,
            double desiredBearing
        );

};
#endif
