#include "power.h"

void PowerManager::initialize() {
    // Initialize the INA219 current sensor.
    // By default the initialization will use the largest range (32V, 2A).  However
    // you can call a setCalibration function to change this range (see comments).
    if (!batteryCurrentMonitor.begin()) {
        //logDebugMessage("Failed to find INA219 chip for battery\n");
    }

}

void PowerManager::getPowerInfo(PowerInfo *powerInfo) {
    // Measure the voltage directly out of Battery Charging circuit.
    float sampleCount = 0;
    float measuredVoltageSum = 0;
    while (sampleCount < POWER_BATTERY_VOLTAGE_NUM_SAMPLES) {
        measuredVoltageSum += analogRead(POWER_BATTERY_VOLTAGE_PIN);
        sampleCount++;
        delay(1);
    }

    // Voltage divider in place
    // R1 = 4.7k Ohms
    // R2 = 1k Ohms
    // Vout = (R2 / (R1 + R2)) * Vin
    // So Vin = Vout / (R2 / (R1 + R2))
    float pinVoltage = ((float)measuredVoltageSum / (float)POWER_BATTERY_VOLTAGE_NUM_SAMPLES * 5.0) / 1023.0;
    powerInfo->batteryVoltage = pinVoltage / (1000.0 / (4700.0 + 1000.0));

    // Measure the present current draw at the Battery Charging circuit
    powerInfo->batteryCurrentDraw = batteryCurrentMonitor.getCurrent_mA();

}
