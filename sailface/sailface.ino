

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

    Serial.println(";--Hello world!--");

    commsManager = new SailFaceCommunication();
    positionManager = new SailFacePositionManagement();
    powerManager = new SailFacePowerManagement();

    helmControl = new SailFaceHelm();
    propControl = new SailFacePropulsion();

    Serial.println(";--3!--");

    commsManager->initialize(&globalStatus);

    Serial.println("COM INIT COMPLETE");

    powerManager->initialize(&globalStatus);

    Serial.println("POW INIT COMPLETE");

    positionManager->initialize(&globalStatus);

    Serial.println("POS INIT COMPLETE");

    helmControl->initialize(&globalStatus);

    Serial.println("HELM INIT COMPLETE");
    propControl->initialize(&globalStatus);

    Serial.println("PROP INIT COMPLETE");
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

    //commsManager->sendDebugMessage("HELLO BT!\r\n");
}

void processBluetoothCommand(char *command) {
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

void pollAndProcessBluetoothCommands(SailFaceStatus *status) {
    char *command = commsManager->pollForBluetoothCommandMessages(status);
    if (command != NULL) {
        processBluetoothCommand(command);
    }
}

void pollAndProcessSatteliteCommands(SailFaceStatus *status) {
    SailFaceCommandMessage satCommand;
    int numMessages = commsManager->pollForIridumCommandMessages(status, &satCommand);
    if (numMessages > 0) {
        if (satCommand.waypointLatitude > 0 || satCommand.waypointLongitude > 0) {
            navigation->setWaypoint(
                satCommand.waypointLatitude,
                satCommand.waypointLongitude);
        }

        if (satCommand.propSpeed >= 0) {
            propControl->setPropellerSpeed(satCommand.propSpeed, status);
        }

        //commsManager->toggleBluetooth(command.bluetoothActive);

        //if (commsManager->iridiumUpdateFrequencySeconds > 0) {
        //  commsManager->setIridiumStatusUpdateFreq(iridiumUpdateFrequencySeconds);
        //}

    }
}

void pollAndProcessRadioCommands(SailFaceStatus *status) {
    SailFaceRadioCommandMessage radioCommand;
    commsManager->pollForCurrentRadioCommand(&radioCommand);

    Serial.print("RADIO CMD ");
    Serial.print(radioCommand.propSpeed);
    Serial.print(" ");
    Serial.println(radioCommand.rudderPosition);

    if (radioCommand.propSpeed > 0) {
        propControl->setPropellerSpeed(radioCommand.propSpeed, status);
    }
    helmControl->setRudderPosition(radioCommand.rudderPosition);
}

void loop(void) {
    positionManager->pollGPSForPosition(&globalStatus);
    powerManager->pollForBatteryStatus(&globalStatus);

    //helmControl->pollForRudderAdjustment(&globalStatus);

    if (globalStatus.bluetoothActive) {
        pollAndProcessBluetoothCommands(&globalStatus);
    }

    if (globalStatus.iridiumActive) {
        pollAndProcessSatteliteCommands(&globalStatus);
    }

    if (globalStatus.radioControlActive) {
        pollAndProcessRadioCommands(&globalStatus);
    }

    writeStatusToSerial(&globalStatus);
}
