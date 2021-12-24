#include "sailface.h"

#include "position.h"
#include "power.h"
#include "helm.h"
#include "propulsion.h"
#include "comms.h"

SailFaceStatus globalStatus;
SailFacePositionManagement *positionManager;
SailFacePowerManagement *powerManager;
SailFaceHelm *helmControl;
SailFacePropulsion *propControl;
SailFaceCommunication *commsManager;

void setup(void) {
    Serial.begin(115200);
    Serial.println(";--Hello World!--");

    commsManager = new SailFaceCommunication();
    positionManager = new SailFacePositionManagement();
    powerManager = new SailFacePowerManagement();
    helmControl = new SailFaceHelm();
    propControl = new SailFacePropulsion();


    positionManager->initialize(&globalStatus);
    //powerManager->initialize(&globalStatus);
    //helmControl->initialize(&globalStatus);
    propControl->initialize(&globalStatus);
    commsManager->initialize(&globalStatus);

}

void logDebugMessage(char *message) {
    //commsManager->sendDebugMessage(message);
}

void writeStatusToSerial(SailFaceStatus *status) {
    positionManager->writeStatusMessage(status);
    powerManager->writeStatusMessage(status);
}

void loop(void) {
    Serial.println(";---Start Loop---");

    // Call each module
    positionManager->pollGPSForPosition(&globalStatus);
    powerManager->pollForBatteryStatus(&globalStatus);
    commsManager->pollForCommandMessages(&globalStatus);

    writeStatusToSerial(&globalStatus);

    delay(1000);
}
