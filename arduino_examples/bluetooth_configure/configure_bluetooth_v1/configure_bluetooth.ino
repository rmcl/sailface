/* Use this sketch to enter AT mode on the Bluetooth module.

https://www.instructables.com/Change-the-Baud-Rate-of-HC-05-Bluetooth-Module-Usi/#:~:text=If%20the%20HC%2D05%20is,Then%20click%20%E2%80%9COpen%E2%80%9D.


*/
#include <Arduino.h>
#include <Wire.h>

HardwareSerial &bluetoothSerial = Serial2;

void setup(void) {
    Serial.begin(9600);
    Serial.println("BEGIN AT CMDS.");
    bluetoothSerial.begin(38400);
}

void loop(void) {
    if (Serial.available() > 0) {
        bluetoothSerial.write(Serial.read());
    }

    if (bluetoothSerial.available() > 0) {
        Serial.write(bluetoothSerial.read());
    }
}
