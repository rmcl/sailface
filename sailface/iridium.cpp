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

    pinMode(IRIDIUM_POWER_PIN, OUTPUT);
    digitalWrite(IRIDIUM_POWER_PIN, HIGH);

    // Setup the Ring Indicator Pin
    pinMode(ROCKBLOCK_RING_PIN, INPUT);

    IridiumSerial.begin(19200);
    modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE);
    modem.adjustATTimeout(45);

    // Default values
    lastTransmitTime = 0;

    // Todo: Commit this to PRAM so it survives power cycles
    updateFrequencyMinutes = 60;

}

void IridiumManager::wakeIridium() {
    digitalWrite(ROCKBLOCK_SLEEP_PIN, HIGH);

    if (bluetooth->isBluetoothActive()) {
        HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
        bluetoothDebug->println("Attempting to begin Iridium modem");
    }

    // Begin satellite modem operation
    int errorCode = modem.begin();
    if (errorCode != ISBD_SUCCESS && errorCode != ISBD_ALREADY_AWAKE) {

        if (bluetooth->isBluetoothActive()) {
            HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
            bluetoothDebug->println(
                String("ERR: Iridium begin failed CODE:") + \
                String(errorCode)
            );

            if (errorCode == ISBD_NO_MODEM_DETECTED) {
                bluetoothDebug->println("No modem detected: check wiring.");
            }
        }

        iridiumActive = false;
    } else {

        // TODO: DO WE WANT THIS!?
        // move this somewhere! SHOULD WE REALLY TURN THIS OFF? OR IS IT ALREADY OFF?
        modem.useMSSTMWorkaround(false);

        pollIridiumSignalQuality();
        iridiumActive = true;

        if (bluetooth->isBluetoothActive()) {
            HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
            bluetoothDebug->println("INFO: Iridium successfully begin.");
        }
    }

}

void IridiumManager::sleepIridium() {
    digitalWrite(ROCKBLOCK_SLEEP_PIN, LOW);
    iridiumActive = false;
}

void IridiumManager::pollIridiumSignalQuality() {
    int errorCode = modem.getSignalQuality(signalQuality);
    if (errorCode != ISBD_SUCCESS) {
        if (bluetooth->isBluetoothActive()) {
            HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
            bluetoothDebug->println(
                String("ERR: Iridium Signal Quality Failure CODE:") + \
                String(errorCode)
            );
        }
        return;
    }

    if (bluetooth->isBluetoothActive()) {
        HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
        bluetoothDebug->println(
            String("INFO: Iridium Signal Quality: ") + \
            String(signalQuality)
        );
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

    size_t txMessageSize = 0;
    if (txMessage != NULL) {
        txMessageSize = sizeof(*txMessage);
    }

    size_t rxBufferSize = sizeof(*rxCommandMessage);
    int errorCode = modem.sendReceiveSBDBinary(
        (uint8_t*)txMessage,
        txMessageSize,
        (uint8_t*)rxCommandMessage,
        rxBufferSize);

    if (errorCode != ISBD_SUCCESS) {
        if (bluetooth->isBluetoothActive()) {
            HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
            bluetoothDebug->println(
                String("ERROR: sendReceiveSBDBinary error") + \
                String(errorCode)
            );
        }

        if (errorCode == ISBD_SENDRECEIVE_TIMEOUT) {
            //sendDebugMessage("Try again with a better view of the sky.");
            //TODO: Maybe we put a retry in place?
        }

        return -1;

    } else {
        if (bluetooth->isBluetoothActive()) {
            HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
            bluetoothDebug->println(
                String("INFO: sendReceiveSBDBinary success") + \
                String(errorCode)
            );
        }

        // Update the last transmit time to be when the Iridium Send/Receive command
        // completes.
        lastTransmitTime = millis();

        int messageCount = modem.getWaitingMessageCount();
        if (rxBufferSize > 0) {
            // If we received a message add it to number of messages waiting.
            messageCount += 1;
        }

        return messageCount;
    }
}

// Poll for messages via the RockBlock Ring Alert Pin
int IridiumManager::pollForCommandMessages() {
    if (iridiumActive) {
        return 0;
    }

    bool shouldTransmit = shouldTransmitStatus();

    int messageCount = pollForIridiumRingAlerts();
    if (messageCount == 0 && !shouldTransmit) {
        return 0;
    }

    if (bluetooth->isBluetoothActive()) {
        HardwareSerial *bluetoothDebug = bluetooth->getBluetoothSerial();
        bluetoothDebug->println("Iridium attempting to RX/TX");
    }

    IridiumStatusMessage statusMessage = buildStatusMessage();

    // Is this required? Can i just do &statusMessage later?
    IridiumStatusMessage *txStatusMessagePtr = &statusMessage;

    IridiumCommandMessage firstReceivedCommand;
    while (true) {
        messageCount = sendStatusReceiveCommandMessage(
            txStatusMessagePtr,
            &firstReceivedCommand);

        if (messageCount > 0) {
            processCommandMessage(&firstReceivedCommand);
        }

        if (messageCount > 1) {
            // there are more messages to retrieve
            // dont send status again
            txStatusMessagePtr = NULL;
        } else {
            // otherwise we have sent and retrieved all messages so we are done
            return 1;
        }
    }


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
