#include "sailface.h"

#include "position.h"
#include "power.h"
#include "helm.h"
#include "propulsion.h"
#include "comms.h"
#include "navigation.h"
#include "bluetooth_command.h"

PositionManager *position;
PowerManager *power;
HelmManager *helm;
PropManager *prop;
CommunicationManager *comms;
BluetoothCommand *bluetooth;
NavigationManager *navigation;


void setup(void) {
    Serial.begin(115200);

    comms = new CommunicationManager();
    position = new PositionManager();
    power = new PowerManager();
    helm = new HelmManager();
    prop = new PropManager();
    navigation = new NavigationManager();

    bluetooth = new BluetoothCommand();

    comms->initialize();
    position->initialize();
    power->initialize();
    helm->initialize();
    prop->initialize();
    navigation->initialize();

    bluetooth->initialize();
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

            if (bearing < -365) {
                helm->pollForRudderAdjustment(
                    curPosition.magneticHeading,
                    curPosition.magneticHeadingVariation,
                    bearing
                );
            }
        }
    }

    bluetooth->pollForBluetoothCommandMessages();

    /*
    //bluetooth->println(curPosition.positionValid);
    bluetooth->print("HEAD: " + String(curPosition.magneticHeading, 5));
    bluetooth->println(" LAT: " + String(curPosition.latitude, 5) + " LONG: " + String(curPosition.longitude, 5));

    PowerInfo powInfo;
    power->getPowerInfo(&powInfo);
    bluetooth->println("BAT VOLT:" + String(powInfo.batteryVoltage) + " Current:" + String(powInfo.batteryCurrentDraw));
    */

    //HardwareSerial *bSerial = bluetooth->getBluetoothSerial();
    //bSerial->println("HELLO");
    //delay(2000);

    /*
    if (globalStatus.bluetoothActive) {
        pollAndProcessBluetoothCommands(&globalStatus);
    }

    if (globalStatus.iridiumActive) {
        //pollAndProcessSatteliteCommands(&globalStatus);
    }

    if (globalStatus.radioControlActive) {
        pollAndProcessRadioCommands(&globalStatus);
    }
    */
}


/*
void writeStatusToSerial(SailFaceStatus *status) {

    // write status messages every 20 seconds
    unsigned long curTime = millis();
    if ((curTime - status->lastStatusMessageSentTime) < 10000) {
        //Serial.println(curTime);
        //Serial.println(status->lastStatusMessageTime);
        return;
    }
    status->lastStatusMessageSentTime = curTime;

}
*/
/*
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
    } else if (command[0] == 'W') {
        // Set a new waypoint
        // Expected format is: "W34197644,-120035801"
        logDebugMessage("SET WAYPOINT: ");

        char *firstVal = strtok(&(command[1]), ",");
        char *secondVal = strtok(NULL, ",");
        if (secondVal == NULL) {
            logDebugMessage("MALFORMED WAYPOINT");
        } else {
            long newLatitude = atol(firstVal);
            long newLongitude = atol(secondVal);

            logDebugMessage(newLatitude,2);
            logDebugMessage(",");
            logDebugMessage(newLongitude,2);
            logDebugMessage("\n");
            navigation->setWaypoint(newLatitude, newLongitude, &globalStatus);
        }

    } else if (command[0] == 'C') {

        logDebugMessage("CURRENT COURSE DETAILS\n");
        logDebugMessage(globalStatus.latitude);
        logDebugMessage(",");
        logDebugMessage(globalStatus.longitude);
        logDebugMessage("\nCURRENT COURSE: ");
        logDebugMessage(globalStatus.course);
        logDebugMessage("\nset waypoint:\n");
        logDebugMessage(globalStatus.waypointLatitude);
        logDebugMessage(",");
        logDebugMessage(globalStatus.waypointLongitude);

        logDebugMessage("\n\nCOMPUTE COURSE TO!");
        logDebugMessage("\nDESIRED BEARING: ");
        logDebugMessage(globalStatus.desiredBearing);
        logDebugMessage("\nDISTANCE TO WAYPOINT: ");
        logDebugMessage(globalStatus.distanceToWaypoint);
        logDebugMessage("\n");
    } else if (command[0] == 'B') {
        int desiredBearing = 0;
        sscanf(&(command[1]), "%d", &desiredBearing);
        logDebugMessage("DESIRED BEARING: ");
        logDebugMessage(desiredBearing);
        logDebugMessage("\n");

        helmControl->setBearingAndEnablePID(desiredBearing, &globalStatus);

    } else if (command[0] == 'N') {
        logDebugMessage("ENABLE PID CONTROL OF RUDDER AND NAVIGATE TO WAYPOINT\n");
        helmControl->setBearingAndEnablePID(globalStatus.desiredBearing, &globalStatus);

    } else if (command[0] == 'M') {
        logDebugMessage("DISABLE PID CONTROL OF RUDDER\n");
        helmControl->disablePID(&globalStatus);

    } else if (command[0] == 'Q') {
        logDebugMessage("IRIDIUM SIGNAL QUALITY: ");
        commsManager->pollIridiumSignalQuality(&globalStatus);
        logDebugMessage(globalStatus.iridiumSignalQuality);
        logDebugMessage("\n");
    } else if (command[0] == 'X') {
        logDebugMessage("SEND STATUS THROUGH IRIDIUM. SIGNAL QUALITY: ");
        logDebugMessage(globalStatus.iridiumSignalQuality);
        logDebugMessage("\n");
        //commsManager->sendIridiumStatusMessage(&globalStatus);
    } else if (command[0] == 'P') {
        logDebugMessage("PUT THE IRIDIUM TO SLEEP\n");
        commsManager->sleepIridium(&globalStatus);
    } else if (command[0] == 'K') {
        logDebugMessage("START IRIDIUM\n");
        commsManager->wakeIridium(&globalStatus);
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
    logDebugMessage("Polling for sat messages!\n");
    int numMessages = commsManager->pollForIridumCommandMessages(status, &satCommand, false);
    if (numMessages > 0) {
        logDebugMessage("AT LEAST ONE SAT MESSAGE RECEIVED!");
        if (satCommand.waypointLatitude > 0 || satCommand.waypointLongitude > 0) {
            navigation->setWaypoint(
                satCommand.waypointLatitude,
                satCommand.waypointLongitude,
                status);
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
*/
