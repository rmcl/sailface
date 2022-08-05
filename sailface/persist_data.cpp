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
        currentData.iridiumActive = false;
        currentData.iridiumTransmitFrequency = 60;
        currentData.iridiumLastTransmitTime = 0;
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

void PersistDataManager::storeWaypoints(Waypoint[] waypoint, int count) {
    int actualWaypointCount = min(MAX_WAYPOINTS, count)

    // Iterate over the total number of waypoints that can be stored and either
    // persist new value or set to zero.
    for (int i = 0; i<MAX_WAYPOINTS; i++) {
        if (i < actualWaypointCount) {
            currentData.waypoints[i] = waypoint[i];
        } else {
            currentData.waypoints[i].latitude = 0;
            currentData.waypoints[i].longitude = 0;
        }
    }
    persist();
}

void PersistDataManager::storeIridiumLastTransmitTime(unsigned long lastTransmitTime) {
    currentData->iridiumLastTransmitTime = lastTransmitTime;
    persist();
}

unsigned long PersistDataManager::getIridiumLastTransmitTime() {
    return currentData->iridiumLastTransmitTime;
}

void PersistDataManager::storeIridiumActive(bool iridiumActive) {
    currentData->iridiumActive = true;
    persist();
}
bool PersistDataManager::getIridiumActive() {
    return currentData->iridiumActive;
}
