/* Adjust the rudder to keep SailFace on a desired bearing.


A little PID theory from (https://playground.arduino.cc/Code/PIDLibrary/)

You tell the PID module what to measure (the "Input",) Where you want that
measurement to be (the "Setpoint",) and the variable to adjust that can make
that happen (the "Output".) The PID then adjusts the output trying to make
the input equal the setpoint.

For reference, in a car, the Input, Setpoint, and Output would be the speed,
desired speed, and gas pedal angle respectively.

In the case of SailFACE,
    Input - Current heading according to GPS module
    SetPoint - Desired Bearing (angle from current position to waypoint)
    and Output - Rudder position.

*/
#include "sailface.h"
#include "helm.h"


// TODO: If we wanna setup the RC receiver for close range driving.
// https://www.youtube.com/watch?v=u0Ft8SB3pkw
//

void SailFaceHelm::initialize(SailFaceStatus *status) {

    helmServo.attach(
        HELM_PWM_PIN,
        HELM_PWM_RANGE_MIN,
        HELM_PWM_RANGE_MAX
    );

    //setRudderPosition(0);

}

/* Set the position of the rudder */
void SailFaceHelm::setRudderPosition(int position) {
    /* Take a range between -10 and 10

    -10 is full to starboard
    10 is full to port

    */
    position = constrain(position, -10, 10);

    float stepSize = 0;
    if (position > 0) {
        stepSize = (RUDDER_SERVO_MAX_PORT - RUDDER_SERVO_CENTER) / 10.0;
    } else {
        stepSize = (RUDDER_SERVO_CENTER - RUDDER_SERVO_MAX_STARBOARD) / 10.0;
    }
    int servoAngle = RUDDER_SERVO_CENTER + int(stepSize * position);

    logDebugMessage("ANGLE TO SERVO");
    logDebugMessage(servoAngle);
    logDebugMessage("\n");
    helmServo.write(servoAngle);
}

void SailFaceHelm::setBearingAndEnablePID(int bearing, SailFaceStatus *status) {

/*
    // if PID control already enabled and if the new bearing is basically
    // same as the old bearing don't reinitialize the PID
    if (status->enablePIDControl) {
        if (abs(status->desiredBearing - bearing) < .5) {
            return;
        }
    }

    // clean up any old PID objects if needed
    disablePID();

    sailface->desiredBearing = bearing;
    sailface->enablePIDControl = true;

    rudderPID = new PID(
        &difference,
        &pidRudderPositionOut,
        &(sailface->desiredBearing),
        Kp, Ki, Kd, DIRECT)

    rudderPID->SetOutputLimits(-10, 10);
*/
}
void SailFaceHelm::disablePID(SailFaceStatus *status) {
    status->enablePIDControl = false;

    if (rudderPID) {
        delete rudderPID;
        rudderPID = NULL;
    }
}

void SailFaceHelm::pollForRudderAdjustment(SailFaceStatus *status) {
/*
    if (status->enablePIDControl == false) {
        return;
    }

    rudderPID->compute()

    int newRudderPosition = int(pidRudderPositionOut);
    setRudderPosition(newRudderPosition);
*/
}
