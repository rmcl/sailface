#include <stdint.h>
#include <Arduino.h>
#include <Servo.h>

#define HELM_PWM_PIN 5
#define HELM_PWM_RANGE_MIN 500
#define HELM_PWM_RANGE_MAX 2500


Servo helmServo;
uint32_t next;
bool rising;

int pos;

void setup() {

    //Serial.begin(9600);

    helmServo.attach(
        HELM_PWM_PIN,
        HELM_PWM_RANGE_MIN,
        HELM_PWM_RANGE_MAX
    );

    next = millis();
    rising = true;

}

void loop() {


    //Serial.print(millis());
    //Serial.print(":");
    //Serial.println(next);

    //helmServo.write(105);
    //delay(15000);
    //helmServo.write(45);
    //delay(15000);

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
    }
}
