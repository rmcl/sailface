/* Library utilizing Arduino's EEPROM to store important information across power cycles.

*/
#ifndef _SAIL_FACE_PERSIST_DATA
#define _SAIL_FACE_PERSIST_DATA

#include <EEPROM.h>

#include "navigation.h"

#define SF_PERSISTED_DATA_ADDRESS 0x0
#define SF_CURRENT_PERSIST_VALID 0x69


//
typedef struct {
    // Store a number that we can check at start up to determine if contents
    // of persistent memory are valid for this version of firmware. By changing
    // the expected number here old data can be invalidated.
    short persistValid;

    short propSpeed;
    short bluetoothActive;

    // Time in seconds that status updates are sent through Iridium.
    int iridiumTransmitFrequency;

    //Waypoint waypoints[10];

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

        void storeIridiumTransmitFrequency(int frequency);
        int getIridiumTransmitFrequency();

        //void storeWaypoints(Waypoint[] waypoints, int count);
        //Waypoint[] getWaypoints();

};

#endif
