/* Library utilizing Arduino's EEPROM to store important information across power cycles.

*/
#ifndef _SAIL_FACE_PERSIST_DATA
#define _SAIL_FACE_PERSIST_DATA

#include <EEPROM.h>

#include "navigation.h"
#include "position.h"

#define SF_PERSISTED_DATA_ADDRESS 0x0
#define SF_CURRENT_PERSIST_VALID 0x69
#define MAX_WAYPOINTS 20


//
typedef struct {
    // Store a number that we can check at start up to determine if contents
    // of persistent memory are valid for this version of firmware. By changing
    // the expected number here old data can be invalidated.
    short persistValid;

    short propSpeed;
    bool bluetoothActive;

    bool iridiumActive;

    // Time in seconds that status updates are sent through Iridium.
    int iridiumTransmitFrequency;
    unsigned long iridiumLastTransmitTime;

    bool isNavigatingToWaypoint;

    int numWaypoints;
    Waypoint waypoints[MAX_WAYPOINTS];

    MPUCalibrationParams calibrationParams;

} PersistedData;

class PersistDataManager {

    private:
        PersistedData currentData;

        void load();
        void persist();

    public:
        void initialize();

        void storePropSpeed(short propSpeed);
        short getPropSeed();

        void storeBluetoothActive(bool bluetoothActive);
        bool getBluetoothAcive();

        void storeIridiumActive(bool iridiumActive);
        bool getIridiumActive();

        void storeIridiumTransmitFrequency(int frequency);
        int getIridiumTransmitFrequency();

        void storeIridiumLastTransmitTime(unsigned long lastTransmitTime);
        unsigned long getIridiumLastTransmitTime();

        void storeIsNavigatingToWaypoint(bool isNavigating);
        bool getIsNavigatingToWaypoint();

        void storeWaypoints(
            Waypoint waypoints[],
            int count,
            bool appendWaypoints);

        // Returns number of waypoints and assigns the position of the array
        // to the provided pointer.
        int getWaypoints(Waypoint *waypoints);

        void storeMPUCalibrationParams(MPUCalibrationParams params);
        MPUCalibrationParams getMPUCalibrationParams();
};

#endif
