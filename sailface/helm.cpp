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
#include "helm.h"
#include "sailface.h"

// TODO: If we wanna setup the RC receiver for close range driving.
// https://www.youtube.com/watch?v=u0Ft8SB3pkw
//

void HelmManager::initialize() {
    helmServo.attach(
        HELM_PWM_PIN,
        HELM_PWM_RANGE_MIN,
        HELM_PWM_RANGE_MAX
    );

    // LOAD PID parameters from persisted data.
    pidParams = persistedData->getPIDParams();

    enablePIDControl = false;
}

/* Update the PID parameters and reinitilize the PID controller to use them. */
void HelmManager::setPIDParams(PIDParams params) {
    pidParams = params;
    persistedData->storePIDParams(params);

    disablePID();
    setBearingAndEnablePID(desiredBearing);
}

PIDParams HelmManager::getPIDParams() {
    return pidParams;
}

/* Set the position of the rudder */
void HelmManager::setRudderPosition(int position) {
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
    helmServo.write(servoAngle);
}

void HelmManager::setBearingAndEnablePID(int bearing) {

    // if PID control already enabled and if the new bearing is basically
    // same as the old bearing don't reinitialize the PID
    if (enablePIDControl == true) {
        if (abs(desiredBearing - bearing) < .5) {
            return;
        }
    }

    // clean up any old PID objects if needed
    disablePID();

    desiredBearing = bearing;
    enablePIDControl = true;

    difference = 0; // can we set this to a constant to start?
    //status->magneticCourse - status->magneticCourseVariation - status->desiredBearing;

    rudderPID = new PID(
        &difference,
        &pidRudderPositionOut,
        &setPoint,
        pidParams.Kp,
        pidParams.Ki,
        pidParams.Kd,
        DIRECT);

    rudderPID->SetMode(AUTOMATIC);
    rudderPID->SetOutputLimits(-10, 10);

}
void HelmManager::disablePID() {
    enablePIDControl = false;

    if (rudderPID) {
        delete rudderPID;
        rudderPID = NULL;
    }
}

void HelmManager::pollForRudderAdjustment(
    double curMagHeading,
    double curMagHeadingVariation,
    double desiredBearing
) {

    // only recompute rudder position every 2 seconds.
    unsigned long curTime = millis();
    if ((curTime - lastAdjustTime) < 2000) {
        return;
    }
    lastAdjustTime = curTime;


    // Either initialize PID controller or reinitialize if we have
    // changed our desired bearing.
    setBearingAndEnablePID(desiredBearing);


    // TODO(rmcl): This likely doesn't quite work. Revisit later.
    difference = curMagHeading - curMagHeadingVariation - desiredBearing;
    rudderPID->Compute();

    int newRudderPosition = int(pidRudderPositionOut);
    setRudderPosition(newRudderPosition);

    /*
    if (bluetooth->isBluetoothActive()) {
        HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
        bluetoothDebug->println(
            String("INFO: NEW RUDDER POS") + String(newRudderPosition) + String("\n") + \
            String("CUR MAG:") + String(curMagHeading) + String(" MAG VAR:") + String(curMagHeadingVariation) + \
            String("Bearing to waypoint: ") + String(desiredBearing)
        );
    }
    */
}
