

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
    //Serial.begin(115200);

    //Serial.println(";--Hello world!--");

    commsManager = new SailFaceCommunication();
    positionManager = new SailFacePositionManagement();
    powerManager = new SailFacePowerManagement();

    helmControl = new SailFaceHelm();
    propControl = new SailFacePropulsion();

    //Serial.println(";--3!--");

    commsManager->initialize(&globalStatus);

    //Serial.println("COM INIT COMPLETE");

    powerManager->initialize(&globalStatus);

    //Serial.println("POW INIT COMPLETE");

    positionManager->initialize(&globalStatus);

    //Serial.println("POS INIT COMPLETE");

    helmControl->initialize(&globalStatus);

    //Serial.println("HELM INIT COMPLETE");
    propControl->initialize(&globalStatus);

    //Serial.println("PROP INIT COMPLETE");
    navigation->initialize(&globalStatus);

    //Serial.println(";--Setup Complete!--");
}

void logDebugMessage(char *message) {
    commsManager->sendDebugMessage(message);
}
void logDebugMessage(int number) {
    char numberMessage[16];
    itoa(number, numberMessage, 10);
    commsManager->sendDebugMessage(numberMessage);
}
void logDebugMessage(uint32_t number) {
    char numberMessage[16];
    itoa(number, numberMessage, 10);
    commsManager->sendDebugMessage(numberMessage);
}
void logDebugMessage(double number) {
    char numberMessage[16];
    ltoa(number, numberMessage, 10);
    commsManager->sendDebugMessage(numberMessage);
}
void logDebugMessage(float number, int num_digits_after_decimal) {
    char numberMessage[50];
    dtostrf(number,5, num_digits_after_decimal, numberMessage);
    commsManager->sendDebugMessage(numberMessage);
}

void logDebugMessage(float number) {
    char numberMessage[50];
    dtostrf(number,5,2,numberMessage);
    commsManager->sendDebugMessage(numberMessage);
}

void writeStatusToSerial(SailFaceStatus *status) {

    // write status messages every 20 seconds
    unsigned long curTime = millis();
    if ((curTime - status->lastStatusMessageSentTime) < 20000) {
        //Serial.println(curTime);
        //Serial.println(status->lastStatusMessageTime);
        return;
    }
    status->lastStatusMessageSentTime = curTime;

    positionManager->writeStatusMessage(commsManager, status);
    powerManager->writeStatusMessage(status);

    //commsManager->sendDebugMessage("HELLO BT!\r\n");
    commsManager->writeStatusMessage(status);
}

void processBluetoothCommand(char *command) {
    logDebugMessage("PROCESS COMMAND: ");
    logDebugMessage(command);
    logDebugMessage("\n");
    if (command[0] == 'E') {
        logDebugMessage("START PROP\n");
        int propLevel = 0;
        sscanf(&(command[1]), "%d", &propLevel);
        logDebugMessage("POWER TO ");
        logDebugMessage(propLevel);
        logDebugMessage("\n");
        propControl->setPropellerSpeed(propLevel, &globalStatus);
    } else if (command[0] == 'S') {
        logDebugMessage("STOP PROP\n");
        propControl->setPropellerSpeed(0, &globalStatus);
    } else if (command[0] == 'R') {
        int degrees = 0;
        sscanf(&(command[1]), "%d", &degrees);
        logDebugMessage("ROTATING TO ");
        logDebugMessage(degrees);
        logDebugMessage("\n");
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

    logDebugMessage("RADIO CMD ");
    logDebugMessage(radioCommand.propSpeed);
    logDebugMessage(" ");
    logDebugMessage(radioCommand.rudderPosition);
    logDebugMessage("\n");

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
