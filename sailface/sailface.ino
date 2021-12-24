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

    positionManager = new SailFacePositionManagement();
    powerManager = new SailFacePowerManagement();
    //helmControl = new SailFaceHelm();
    propControl = new SailFacePropulsion();
    commsManager = new SailFaceCommunication();

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
    positionManager->poll(&globalStatus);
    powerManager->poll(&globalStatus);
    /*
    helmControl->poll(&globalStatus);
    */
    propControl->poll(&globalStatus);
    commsManager->poll(&globalStatus);

    writeStatusToSerial(&globalStatus);

    delay(1000);
}
