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

        currentData.calibrationParams = {0};

        currentData.isNavigatingToWaypoint = false;
        currentData.numWaypoints = 0;

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

void PersistDataManager::storeIsNavigatingToWaypoint(bool isNavigating) {
    currentData.isNavigatingToWaypoint = isNavigating;
    persist();
}

bool PersistDataManager::getIsNavigatingToWaypoint() {
    return currentData.isNavigatingToWaypoint;
}

// Persist waypoints to EEPROM.
// appendWaypoints - if true, add new waypoints to existing waypoints; if false
// overwrite existing waypoints
void PersistDataManager::storeWaypoints(
    Waypoint waypoint[],
    int count,
    bool appendWaypoints
) {
    int numNewWaypoints = min(
        MAX_WAYPOINTS - currentData.numWaypoints,
        count);

    int index = currentData.numWaypoints;
    if (appendWaypoints == false) {
        index = 0;
    }

    for (; index < MAX_WAYPOINTS; index++) {
        if (index < numNewWaypoints) {
            currentData.waypoints[index] = waypoint[index];
        } else {
            currentData.waypoints[index].latitude = 0;
            currentData.waypoints[index].longitude = 0;
            currentData.waypoints[index].allowedRadiusMeters = 0;
        }
    }

    currentData.numWaypoints += numNewWaypoints;

    persist();
}
int PersistDataManager::getWaypoints(Waypoint *waypoints) {
    waypoints = currentData.waypoints;
    return currentData.numWaypoints;
}

void PersistDataManager::storeIridiumLastTransmitTime(unsigned long lastTransmitTime) {
    currentData.iridiumLastTransmitTime = lastTransmitTime;
    persist();
}

unsigned long PersistDataManager::getIridiumLastTransmitTime() {
    return currentData.iridiumLastTransmitTime;
}

void PersistDataManager::storeIridiumActive(bool iridiumActive) {
    currentData.iridiumActive = true;
    persist();
}
bool PersistDataManager::getIridiumActive() {
    return currentData.iridiumActive;
}

void PersistDataManager::storeMPUCalibrationParams(MPUCalibrationParams params) {
    currentData.calibrationParams = params;
    persist();
}

MPUCalibrationParams PersistDataManager::getMPUCalibrationParams() {
    return currentData.calibrationParams;
}
