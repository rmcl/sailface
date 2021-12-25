#include "sailface.h"
#include "helm.h"

// 155 - All the way to PORT
// 120 - Centered
// 85 - All the way to STARBOARD
#define RUDDER_SERVO_MAX_PORT 155
#define RUDDER_SERVO_MAX_STARBOARD 85
#define RUDDER_SERVO_CENTER 120

void SailFaceHelm::initialize(SailFaceStatus *status) {
    helmServo.attach(
        HELM_PWM_PIN,
        HELM_PWM_RANGE_MIN,
        HELM_PWM_RANGE_MAX
    );
}

/* Set the position of the rudder */
void SailFaceHelm::setRudderPosition(int position) {
    /* Take a range between -10 and 10

    -10 is full to starboard
    10 is full to port

    */

    float stepSize = 0;
    if (position > 0) {
        stepSize = (RUDDER_SERVO_MAX_PORT - RUDDER_SERVO_CENTER) / 10.0;
    } else {
        stepSize = (RUDDER_SERVO_CENTER - RUDDER_SERVO_MAX_STARBOARD) / 10.0;
    }
    int servoAngle = RUDDER_SERVO_CENTER + int(stepSize * position);

    Serial.println(servoAngle);
    helmServo.write(servoAngle);
}
