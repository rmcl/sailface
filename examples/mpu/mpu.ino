#include <Arduino.h>
#include "MPU9250.h"

#define AVERAGE_HEADING_BUFFER_SIZE 50
MPU9250 mpu;
uint32_t prev_ms;
float averageHeadingBuffer[AVERAGE_HEADING_BUFFER_SIZE];
int averageHeadingBufferIdx = 0;


void setup() {
    Serial.begin(115200);

    Wire.begin();
    delay(2000);
    
    if (!mpu.setup(0x68)) {
            Serial.println(
                String("ERR: MPU connection failed. Please check wiring")
            );
    }

    //mpu.setMagBias(171.38, 633.19, -22.61);
    //mpu.setMagScale(1.75, 0.47, 3.22);

    int averageHeadingBufferIdx = 0;
    for ( int n = 0; n < AVERAGE_HEADING_BUFFER_SIZE; ++n ) {
        averageHeadingBuffer[n] = 0;
    }
}

void loop() {

    if (mpu.update()) {
        if (millis() > prev_ms + 100) {
            float magX = mpu.getMagX();
            float magY = mpu.getMagY();
            float instantHeading;
            if (magY > 0) {
                instantHeading = 90 - atan(magX/magY)*(180/M_PI);
            } else if (magY < 0) {
                instantHeading = 270 - atan(magX/magY)*(180/M_PI);
            } else if (magY == 0 && magX < 0) {
                instantHeading = 180;
            } else if (magY == 0 && magX > 0) {
                instantHeading = 0;
            }

            averageHeadingBuffer[averageHeadingBufferIdx] = instantHeading;
            averageHeadingBufferIdx = (averageHeadingBufferIdx + 1) % AVERAGE_HEADING_BUFFER_SIZE;

            float sum = 0;
            for ( int idx = 0; idx < AVERAGE_HEADING_BUFFER_SIZE; ++idx ) {
                sum += averageHeadingBuffer[idx];
            }
            float magneticHeading = sum / AVERAGE_HEADING_BUFFER_SIZE;

            Serial.println(magneticHeading);

            prev_ms = millis();
        }
    }
}
