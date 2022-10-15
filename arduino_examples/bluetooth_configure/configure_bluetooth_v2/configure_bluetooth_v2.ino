/*

IMPORTANT: When you want to enter AT mode. Need to set the "EN" pin high to 3.3v


References
* https://www.instructables.com/Change-the-Baud-Rate-of-HC-05-Bluetooth-Module-Usi/#:~:text=If%20the%20HC%2D05%20is,Then%20click%20%E2%80%9COpen%E2%80%9D.
* https://www.roboindia.com/tutorials
*/

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(5, 6); // RX | TX

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("Enter AT commands:");
  
}

void loop()
{
    if (BTSerial.available())    // read from HC-05 and send to Arduino Serial Monitor
      Serial.write(BTSerial.read());
  
    if (Serial.available())     // Keep reading from Arduino Serial Monitor and send to HC-05
      BTSerial.write(Serial.read());

}
