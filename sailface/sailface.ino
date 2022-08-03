#include "sailface.h"

#include "position.h"
#include "power.h"
#include "helm.h"
#include "propulsion.h"
#include "iridium.h"
#include "navigation.h"
#include "bluetooth_command.h"

PositionManager *position;
PowerManager *power;
HelmManager *helm;
PropManager *prop;
IridiumManager *iridium;
BluetoothCommand *bluetooth;
NavigationManager *navigation;


void setup(void) {
    Serial.begin(115200);

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

    Serial.println("setup complete");
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

    //Serial.println("command poll");

    bluetooth->pollForBluetoothCommandMessages();
    //iridium->pollForCommandMessages();

    //Serial.println("command poll complete");


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
