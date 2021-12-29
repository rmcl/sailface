

#include "sailface.h"

#include "position.h"
#include "power.h"
#include "helm.h"
#include "propulsion.h"
#include "comms.h"
#include "navigation.h"

SailFaceStatus globalStatus;
SailFacePositionManagement *positionManager;
SailFacePowerManagement *powerManager;
SailFaceHelm *helmControl;
SailFacePropulsion *propControl;
SailFaceCommunication *commsManager;
SailFaceNavigation *navigation;

void setup(void) {
    Serial.begin(115200);
    Serial.println(";--Hello World!--");

    commsManager = new SailFaceCommunication();
    positionManager = new SailFacePositionManagement();

    Serial.println(";--1!--");

    powerManager = new SailFacePowerManagement();

    Serial.println(";--2!--");

    helmControl = new SailFaceHelm();
    propControl = new SailFacePropulsion();

    Serial.println(";--3!--");

    commsManager->initialize(&globalStatus);
    powerManager->initialize(&globalStatus);
    positionManager->initialize(&globalStatus);
    helmControl->initialize(&globalStatus);
    propControl->initialize(&globalStatus);

    navigation->initialize(&globalStatus);

    Serial.println(";--Setup Complete!--");
}

void logDebugMessage(char *message) {
    //commsManager->sendDebugMessage(message);
}

void writeStatusToSerial(SailFaceStatus *status) {

    // write status messages every 1.5 seconds
    unsigned long curTime = millis();
    if ((curTime - status->lastStatusMessageTime) < 2000) {
        //Serial.println(curTime);
        //Serial.println(status->lastStatusMessageTime);
        return;
    }
    status->lastStatusMessageTime = curTime;

    positionManager->writeStatusMessage(commsManager, status);
    powerManager->writeStatusMessage(status);

    commsManager->sendDebugMessage("HELLO BT!\r\n");
}

void processCommand(char *command) {
    Serial.println("PROCESS COMMAND");
    Serial.println(command);
    if (command[0] == 'E') {
        Serial.println("START PROP");
        int propLevel = 0;
        sscanf(&(command[1]), "%d", &propLevel);
        Serial.print("POWER TO ");
        Serial.println(propLevel);
        propControl->setPropellerSpeed(propLevel, &globalStatus);
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
    //Serial.println(";---Start Loopz---");

    positionManager->pollGPSForPosition(&globalStatus);
    powerManager->pollForBatteryStatus(&globalStatus);
    char *command = commsManager->pollForCommandMessages(&globalStatus);
    if (command != NULL) {
        processCommand(command);
    }

    writeStatusToSerial(&globalStatus);

    //delay(1000);
}
