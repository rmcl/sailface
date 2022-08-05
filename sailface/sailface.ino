#include "sailface.h"

#include "position.h"
#include "power.h"
#include "helm.h"
#include "propulsion.h"
#include "iridium.h"
#include "navigation.h"
#include "bluetooth_command.h"

PersistDataManager *persistedData;
PositionManager *position;
PowerManager *power;
HelmManager *helm;
PropManager *prop;
IridiumManager *iridium;
BluetoothCommand *bluetooth;
NavigationManager *navigation;


void setup(void) {
    // Uncomment for debug via Serial
    //Serial.begin(115200);

    position = new PositionManager();
    power = new PowerManager();
    helm = new HelmManager();
    prop = new PropManager();
    navigation = new NavigationManager();
    bluetooth = new BluetoothCommand();
    iridium = new IridiumManager();

    position->initialize();
    power->initialize();
    helm->initialize();
    prop->initialize();
    navigation->initialize();
    bluetooth->initialize();
    iridium->initialize();
}

void loop(void) {
    position->pollGPSForPosition();
    position->pollForMPU();

    PositionInfo curPosition = position->getCurPosition();
    if (curPosition.positionValid) {
        if (navigation->isNavigatingToWaypoint()) {
            double bearing = navigation->computeBearingToNextWaypoint(
                curPosition.latitude,
                curPosition.longitude
            );

            /// figure out a more appopriate error state
            if (bearing >= -365) {
                helm->pollForRudderAdjustment(
                    curPosition.magneticHeading,
                    curPosition.magneticHeadingVariation,
                    bearing
                );
            }
        }
    }

    bluetooth->pollForBluetoothCommandMessages();
    //iridium->pollForCommandMessages(false);

    /*
    Todo: think about how we want to incorporate this later.
    if (globalStatus.radioControlActive) {
        pollAndProcessRadioCommands(&globalStatus);
    }
    */
}
