

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

    commsManager->initialize(&globalStatus);
    powerManager->initialize(&globalStatus);
    positionManager->initialize(&globalStatus);
    helmControl->initialize(&globalStatus);
    propControl->initialize(&globalStatus);


}

void logDebugMessage(char *message) {
    //commsManager->sendDebugMessage(message);
}

void writeStatusToSerial(SailFaceStatus *status) {
    positionManager->writeStatusMessage(commsManager, status);
    powerManager->writeStatusMessage(status);
}

void processCommand(char *command) {
    Serial.println("PROCESS COMMAND");
    Serial.println(command);
    if (command[0] == 'E') {
        Serial.println("START PROP");
        propControl->setPropellerSpeed(128, &globalStatus);
    } else if (command[0] == 'S') {
        Serial.println("STOP PROP");
        propControl->setPropellerSpeed(0, &globalStatus);
    } else if (command[0] == 'R') {
        int degrees = 0;
        sscanf(&(command[1]), "%d", &degrees);
        Serial.print("ROTATING TO ");
        Serial.println(degrees);
        helmControl->setRudderPosition(degrees);
    }
}

void loop(void) {
    Serial.println(";---Start Loopz---");

    // Call each module
    positionManager->pollGPSForPosition(&globalStatus);
    powerManager->pollForBatteryStatus(&globalStatus);
    char *command = NULL; //commsManager->pollForCommandMessages(&globalStatus);
    if (command != NULL) {
        processCommand(command);
    }

    writeStatusToSerial(&globalStatus);

    delay(1000);
}
