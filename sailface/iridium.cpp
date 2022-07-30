/*

The goal of this module is to send and receive messages via the Iridium Short-Burst
Data services.

*/
#include "sailface.h"
#include "iridium.h"


void IridiumManager::initialize() {
    // SET THE SLEEP PIN TO OUTPUT
    pinMode(ROCKBLOCK_SLEEP_PIN, OUTPUT);
    digitalWrite(ROCKBLOCK_SLEEP_PIN, HIGH);

    // Setup the Ring Indicator Pin
    pinMode(ROCKBLOCK_RING_PIN, INPUT);

    IridiumSerial.begin(19200);
    modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE);
    modem.adjustATTimeout(45);

    // Default values
    lastTransmitTime = 0;

    // Todo: Commit this to PRAM so it survives power cycles
    updateFrequencyMinutes = 60;

    // Begin satellite modem operation
    int errorCode = modem.begin();
    if (errorCode != ISBD_SUCCESS) {
        /*logDebugMessage("Begin failed: error ");
        logDebugMessage(errorCode);
        logDebugMessage("\n");
        if (errorCode == ISBD_NO_MODEM_DETECTED) {
            logDebugMessage("No modem detected: check wiring.");
        }
        */

        iridiumActive = false;
    } else {

        // TODO: DO WE WANT THIS!?
        // move this somewhere! SHOULD WE REALLY TURN THIS OFF? OR IS IT ALREADY OFF?
        modem.useMSSTMWorkaround(false);

        pollIridiumSignalQuality();
        iridiumActive = true;
    }
}

void IridiumManager::wakeIridium() {
    digitalWrite(ROCKBLOCK_SLEEP_PIN, HIGH);
    iridiumActive = true;
}

void IridiumManager::sleepIridium() {
    digitalWrite(ROCKBLOCK_SLEEP_PIN, LOW);
    iridiumActive = false;
}

void IridiumManager::pollIridiumSignalQuality() {
    int errorCode = modem.getSignalQuality(signalQuality);
    if (errorCode != ISBD_SUCCESS) {
        /*logDebugMessage("SignalQuality failed: error ");
        logDebugMessage(errorCode);
        logDebugMessage("\n");*/
        return;
    }
}

// The IridiumSBD will set the "Ring" pin to high if their are messages
// waiting for download. Return the number of messages waiting for
// download.
int IridiumManager::pollForIridiumRingAlerts() {
    bool ring = modem.hasRingAsserted();
    if (ring) {
        return modem.getWaitingMessageCount();
    }
    return 0;
}


IridiumStatusMessage IridiumManager::buildStatusMessage() {
    IridiumStatusMessage message;

    PositionInfo curPosition = position->getCurPosition();
    Waypoint nextWaypoint = navigation->getNextWaypoint();

    PowerInfo powerInfo;
    power->getPowerInfo(&powerInfo);

    message.batteryVoltage = powerInfo.batteryVoltage * 10; // multiple by ten to convert to short int
    message.latitude = curPosition.latitude;
    message.longitude = curPosition.longitude;
    message.waypointLatitude = nextWaypoint.latitude;
    message.waypointLongitude = nextWaypoint.longitude;
    message.propSpeed = prop->getPropellerSpeed();
    return message;
}

bool IridiumManager::shouldTransmitStatus() {
    unsigned long curTime = millis();
    if ((curTime - lastTransmitTime) < (updateFrequencyMinutes * 60000)) {
        return false;
    }
    return true;
}

//
// Send and Receive messages from Iridium.
int IridiumManager::sendStatusReceiveCommandMessage(
    IridiumStatusMessage *txMessage,
    IridiumCommandMessage *rxCommandMessage
) {

    int txMessageSize = 0;
    if (txMessage != NULL) {
        txMessageSize = sizeof(*txMessage);
    }
    int rxMessageSize = sizeof(*rxCommandMessage);
    int errorCode = modem.sendReceiveSBDBinary(
        (uint8_t*)txMessage,
        txMessageSize,
        (uint8_t*)rxCommandMessage,
        &rxMessageSize);

    // Update the last transmit time to be when the Iridium Send/Receive command
    // completes.
    lastTransmitTime = millis();

    if (errorCode != ISBD_SUCCESS) {
        //sendDebugMessage("sendReceiveSBDBinary failed error:");
        //char numberMessage[16];
        //itoa(errorCode, numberMessage, 10);
        //sendDebugMessage(numberMessage);
        //sendDebugMessage("\n");

        if (errorCode == ISBD_SENDRECEIVE_TIMEOUT) {
            //sendDebugMessage("Try again with a better view of the sky.");
        }

        return -1;

    } else {
        //sendDebugMessage("Iridium successfully sent/recv message.");
        //status->lastIridiumStatusMessageSentTime = status->time;

        int messageCount = modem.getWaitingMessageCount();
        if (rxMessageSize > 0) {
            messageCount += 1;
        }

        return messageCount;
    }
}

// Poll for messages via the RockBlock Ring Alert Pin

int IridiumManager::pollForCommandMessages() {
    if (iridiumActive) {
        return;
    }

    bool shouldTransmit = shouldTransmitStatus();

    int messageCount = pollForIridiumRingAlerts();
    if (messageCount == 0 && !shouldTransmit) {
        return;
    }

    IridiumStatusMessage statusMessage = buildStatusMessage();

    // Is this required? Can i just do &statusMessage later?
    IridiumStatusMessage *txStatusMessagePtr = &statusMessage;

    IridiumCommandMessage firstReceivedCommand;
    messageCount = sendStatusReceiveCommandMessage(
        txStatusMessagePtr,
        &firstReceivedCommand);

    processCommandMessage(&firstReceivedCommand);

    // Current message + some number of outstanding messages.
    return 1 + messageCount;
}


void IridiumManager::processCommandMessage(IridiumCommandMessage *commandMessage) {
    prop->setPropellerSpeed(commandMessage->propSpeed);

    updateFrequencyMinutes = commandMessage->updateFrequencyMinutes;

    switch(commandMessage->waypointAction) {
        case 2:
            // Append new waypoint to existing.
            //TODO SUPPORT MULTIPLE WAYPOINTS!!!!!!!!!!!!!!!!!!!
            navigation->setWaypoint(
                commandMessage->waypointLatitude,
                commandMessage->waypointLongitude);

            break;
        case 1:
            // Clear existing waypoints and append
            navigation->setWaypoint(
                commandMessage->waypointLatitude,
                commandMessage->waypointLongitude);
            break;
        case 0:
        default:
            break;
    }
}

/* Iridium Debug Routines

void ISBDConsoleCallback(IridiumSBD *device, char c) {
    Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c) {
    Serial.write(c);
}

*/
