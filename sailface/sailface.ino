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
    helmControl = new SailFaceHelm();
    propControl = new SailFacePropulsion();
    commsManager = new SailFaceCommunication();

    positionManager->initialize(&globalStatus);
    powerManager->initialize(&globalStatus);
    helmControl->initialize(&globalStatus);
    propControl->initialize(&globalStatus);
    commsManager->initialize(&globalStatus);


    //while (!Serial) {
    //    delay(1);
    //}
}

void logDebugMessage(char *message) {
    commsManager->sendDebugMessage(message);
}

void writeStatusToSerial(SailFaceStatus *status) {
    positionManager->writeStatusMessage(status);
    powerManager->writeStatusMessage(status);
}

/*
void writeStatusToSerialHumanReadable(SailFaceStatus *status) {
    Serial.println(";--Position--");
    Serial.print(";position is valid:"); Serial.print(status->positionValid); Serial.println("");
    Serial.print(";speed:"); Serial.print(status->speed); Serial.println("");
    Serial.print(";course:"); Serial.print(status->course); Serial.println("");
    Serial.print(";lat:"); Serial.print(status->latitude, 6); Serial.println("");
    Serial.print(";long:"); Serial.print(status->longitude, 6); Serial.println("");
    Serial.print(";age:"); Serial.print(status->gpsFixAge); Serial.println("");
    Serial.println(";---");
}
*/

void loop(void) {
    //Serial.println(";---Start Loop---");

    // Call each module
    positionManager->poll(&globalStatus);
    powerManager->poll(&globalStatus);
    helmControl->poll(&globalStatus);
    propControl->poll(&globalStatus);
    commsManager->poll(&globalStatus);

    writeStatusToSerial(&globalStatus);

    delay(1000);
}
