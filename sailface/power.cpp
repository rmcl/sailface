#include "sailface.h"
#include "position.h"

#include "power.h"

void SailFacePowerManagement::initialize(SailFaceStatus *status) {
    status->batteryVoltage = 0;
    status->batteryCurrentDraw = 0;
    status->solarPanelCurrent = 0;

    // Initialize the INA219 current sensor.
    // By default the initialization will use the largest range (32V, 2A).  However
    // you can call a setCalibration function to change this range (see comments).
    if (!batteryCurrentMonitor.begin()) {
        Serial.println("Failed to find INA219 chip for battery");
    }

    if (!solarPanelCurrentMonitor.begin()) {
        Serial.println("Failed to find INA219 chip for solar panel");
    }
    // To use a slightly lower 32V, 1A range (higher precision on amps):
    //ina219.setCalibration_32V_1A();
    // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
    //ina219.setCalibration_16V_400mA();
}

void SailFacePowerManagement::pollForBatteryStatus(SailFaceStatus *status) {
    // Measure the voltage directly out of Battery Charging circuit.
    float sampleCount = 0;
    float measuredVoltageSum = 0;
    while (sampleCount < POWER_BATTERY_VOLTAGE_NUM_SAMPLES) {
        measuredVoltageSum += analogRead(POWER_BATTERY_VOLTAGE_PIN);
        sampleCount++;
        delay(1);
    }

    status->batteryVoltage = ((float)measuredVoltageSum / (float)POWER_BATTERY_VOLTAGE_NUM_SAMPLES * 5.0) / 1023.0;

    // Measure the present current draw at the Battery Charging circuit
    status->batteryCurrentDraw = batteryCurrentMonitor.getCurrent_mA();
    status->solarPanelCurrent = solarPanelCurrentMonitor.getCurrent_mA();

}

void SailFacePowerManagement::writeStatusMessage(SailFaceStatus *status) {
    Serial.print("P:");
    Serial.print(status->batteryVoltage);
    Serial.print(",");
    Serial.print(status->batteryCurrentDraw);
    Serial.print(",");
    Serial.print(status->solarPanelCurrent);
    Serial.println("");
}
