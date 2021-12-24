#include "sailface.h"
#include "helm.h"


void SailFaceHelm::initialize(SailFaceStatus *status) {
    helmServo.attach(
        HELM_PWM_PIN,
        HELM_PWM_RANGE_MIN,
        HELM_PWM_RANGE_MAX
    );
}

/* Set the position of the rudder */
void SailFaceHelm::setRudderPosition(int position) {

    //TODO: Figure out the range of the rudder

    helmServo.write(position);
}
