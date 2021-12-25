#include <SoftwareSerial.h>
#include <Adafruit_INA219.h>

SoftwareSerial EEBlue(10, 11); // RX | TX
Adafruit_INA219 batteryCurrentMonitor;

void setup() {

    Serial.begin(9600);

    EEBlue.begin(9600);
    //Default Baud for comm, it may be different for your Module.
    Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");

    // Initialize the INA219 current sensor.
    // By default the initialization will use the largest range (32V, 2A).  However
    // you can call a setCalibration function to change this range (see comments).
    if (!batteryCurrentMonitor.begin()) {
        Serial.println("Failed to find INA219 chip for battery");
    }

    batteryCurrentMonitor.setCalibration_16V_400mA();

}

void loop() {

    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
    float power_mW = 0;

    shuntvoltage = batteryCurrentMonitor.getShuntVoltage_mV();
    busvoltage = batteryCurrentMonitor.getBusVoltage_V();
    current_mA = batteryCurrentMonitor.getCurrent_mA();
    power_mW = batteryCurrentMonitor.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
    Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
    Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    Serial.println("");

    EEBlue.write(current_mA);
    //float tmp = 1000.5;
    //EEBlue.write(tmp);
    //EEBlue.println("Hello!!!!");

    delay(1000);



    /*
    // Feed any data from bluetooth to Terminal.
    if (EEBlue.available()) {
        Serial.write(EEBlue.read());
    }

    // Feed all data from termial to bluetooth
    if (Serial.available()) {
       EEBlue.write(Serial.read());
    }
    */
}
