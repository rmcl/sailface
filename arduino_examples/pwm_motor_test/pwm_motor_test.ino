#include <PWM.h>

#define PRIMARY_PROP_PWM_PIN A3

void setup() {
  Serial.begin(9600);

  InitTimersSafe();
  SetPinFrequencySafe(PRIMARY_PROP_PWM_PIN,4000);

  // put your setup code here, to run once:
  pinMode(PRIMARY_PROP_PWM_PIN, OUTPUT);
  pinMode(5, INPUT);
}

void loop() {
  pwmWrite(PRIMARY_PROP_PWM_PIN, 140);
  Serial.println("on");
  delay(5000);

  //analogRead(5
  
  pwmWrite(PRIMARY_PROP_PWM_PIN, 0);
  Serial.println("off");
  delay(5000);
}
