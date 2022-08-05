#include "persist_data.h"

void PersistDataManager::initialize() {
    load();
}

void PersistDataManager::load() {
    EEPROM.get(SF_PERSISTED_DATA_ADDRESS, currentData);

    if (currentData.persistValid != SF_CURRENT_PERSIST_VALID) {
        // Data is not valid. Set to some reasonable defaults
        currentData.propSpeed = 0;
        currentData.bluetoothActive = true;
        currentData.iridiumTransmitFrequency = 60;
    }
}

void PersistDataManager::persist() {
    // set the persist valid number so that load will consider this data valid on
    // the next load / power cycle.
    currentData.persistValid = SF_CURRENT_PERSIST_VALID;

    EEPROM.put(SF_PERSISTED_DATA_ADDRESS, currentData);
}

void PersistDataManager::storePropSpeed(short propSpeed) {
    currentData.propSpeed = propSpeed;
    persist();
}
short PersistDataManager::getPropSeed() {
    return currentData.propSpeed;
}

void PersistDataManager::storeBluetoothActive(bool bluetoothActive) {
    currentData.bluetoothActive = bluetoothActive;
    persist();
}
bool PersistDataManager::getBluetoothAcive() {
    return currentData.bluetoothActive;
}

void PersistDataManager::storeIridiumTransmitFrequency(int frequency) {
    currentData.iridiumTransmitFrequency = frequency;
    persist();
}

int PersistDataManager::getIridiumTransmitFrequency() {
    return currentData.iridiumTransmitFrequency;
}
