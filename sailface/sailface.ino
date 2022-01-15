

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

void logDebugMessage(char c) {
    char msg[10];
    msg[0] = c;
    msg[1] = '\0';
    commsManager->sendDebugMessage(msg);
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

void logDebugMessage(long number) {
    char numberMessage[16];
    ltoa(number, numberMessage, 10);
    commsManager->sendDebugMessage(numberMessage);
}

/* dtoa implementation from this thread:
    https://forum.arduino.cc/t/convert-double-to-char/525251/5
*/
char* dtoa(double dN, char *cMJA, int iP) {
  char *ret = cMJA; long lP=1; byte bW=iP;
  while (bW>0) { lP=lP*10;  bW--;  }
  long lL = long(dN); double dD=(dN-double(lL))* double(lP);
  if (dN>=0) { dD=(dD + 0.5);  } else { dD=(dD-0.5); }
  long lR=abs(long(dD));  lL=abs(lL);
  if (lR==lP) { lL=lL+1;  lR=0;  }
  if ((dN<0) & ((lR+lL)>0)) { *cMJA++ = '-';  }
  ltoa(lL, cMJA, 10);
  if (iP>0) { while (*cMJA != '\0') { cMJA++; } *cMJA++ = '.'; lP=10;
  while (iP>1) { if (lR< lP) { *cMJA='0'; cMJA++; } lP=lP*10;  iP--; }
  ltoa(lR, cMJA, 10); }  return ret;
}

void logDebugMessage(double number) {
    char numberMessage[16];
    dtoa(number, numberMessage, 16);
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
    //commsManager->writeStatusMessage(status);
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

    } else if (command[0] == 'Q') {
        logDebugMessage("IRIDIUM SIGNAL QUALITY: ");
        commsManager->pollIridiumSignalQuality(&globalStatus);
        logDebugMessage(globalStatus.iridiumSignalQuality);
        logDebugMessage("\n");
    } else if (command[0] == 'X') {
        logDebugMessage("SEND STATUS THROUGH IRIDIUM. SIGNAL QUALITY: ");
        logDebugMessage(globalStatus.iridiumSignalQuality);
        logDebugMessage("\n");
        commsManager->sendIridiumStatusMessage(&globalStatus);
    } else if (command[0] == 'R') {

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
    int numMessages = commsManager->pollForIridumCommandMessages(status, &satCommand);
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

void loop(void) {
    positionManager->pollGPSForPosition(&globalStatus);
    powerManager->pollForBatteryStatus(&globalStatus);
    navigation->recomputeCourseToWaypoint(&globalStatus);

    //helmControl->pollForRudderAdjustment(&globalStatus);

    if (globalStatus.bluetoothActive) {
        pollAndProcessBluetoothCommands(&globalStatus);
    }

    if (globalStatus.iridiumActive) {
        //pollAndProcessSatteliteCommands(&globalStatus);
    }

    if (globalStatus.radioControlActive) {
        pollAndProcessRadioCommands(&globalStatus);
    }

    writeStatusToSerial(&globalStatus);
}
