#include "sailface.h"
#include "helm.h"


void SailFaceHelm::initialize(SailFaceStatus *status) {
    helmServo.attach(
        HELM_PWM_PIN,
        HELM_PWM_RANGE_MIN,
        HELM_PWM_RANGE_MAX
    );

    next = millis() + 500;
}

void SailFaceHelm::poll(SailFaceStatus *status) {
    static bool rising = true;

    Serial.print(millis());
    Serial.print(":");
    Serial.println(next);

    //helmServo.write(105);
    //delay(15000);
    //helmServo.write(45);
    //delay(15000);
    /*
    if (millis() > next) {

        if (rising) {
            helmServo.write(150);
            rising = false;
        } else {
            helmServo.write(30);
            rising = true;
        }

        // repeat again in 5 seconds.
        next += 5000;
    }*/
}
