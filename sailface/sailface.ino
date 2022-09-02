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

    persistedData = new PersistDataManager();
    position = new PositionManager();
    power = new PowerManager();
    helm = new HelmManager();
    prop = new PropManager();
    navigation = new NavigationManager();
    bluetooth = new BluetoothCommand();
    iridium = new IridiumManager();

    persistedData->initialize();
    position->initialize();
    power->initialize();
    helm->initialize();
    prop->initialize();
    navigation->initialize();
    bluetooth->initialize();
    iridium->initialize();
}

void loop(void) {
    sailfaceMainLoop(false);
}

bool ISBDCallback() {
    sailfaceMainLoop(true);

    // Do not cancel the Iridium transmit operation.
    return true;
}

/*
 * The main loop - Perform actions for each of the subsystems
 * The reason we are not using the Arduino built-in loop method is that the
 * IridiumSBD library blocks for 1-10 minutes as it attempts to transmit messages
 * to the sattelite. During that time it repeatedly calls the ISBDCallback which we
 * define above. ISBDCallback will call sailfaceMainLoop so that operations can
 * continue as the Iridium modem attempts to transmit.
 * More information in the ISBD docs:
 * https://github.com/mikalhart/IridiumSBD/blob/master/extras/
    IridiumSBD%20Arduino%20Library%20Documentation.pdf
 */
void sailfaceMainLoop(bool iridiumBusy) {
    position->pollGPSForPosition();
    position->pollForMPU();

    navigation->pollForNavigationAdjustments();

    bluetooth->pollForBluetoothCommandMessages();

    if (!iridiumBusy) {
        iridium->pollForCommandMessages(false);
    }

    /*
    Todo: think about how we want to incorporate this later.
    if (globalStatus.radioControlActive) {
        pollAndProcessRadioCommands(&globalStatus);
    }
    */
}
