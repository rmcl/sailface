/*

Control SailFACE via a Radio transmitter

References
* Source: https://www.youtube.com/watch?v=Bx0y1qyLHQ4

*/

#include "radio.h"

void RadioManager::initialize() {

    // Set radio contorl pins for INPUT.
    pinMode(RADIO_CONTROL_PROP_SPEED_PIN, INPUT);
    pinMode(RADIO_CONTROL_RUDDER_CONTROL_PIN, INPUT);

    radioControlActive = false;
}

void RadioManager::startRadioControl() {
    radioControlActive = true;
}

void RadioManager::stopRadioControl() {
    radioControlActive = false;
}

void RadioManager::pollForCurrentRadioCommand(SailFaceRadioCommandMessage *radioCommand) {
    int propSpeedPulseWidth = pulseIn(
        RADIO_CONTROL_PROP_SPEED_PIN, HIGH);
    int rudderPositionPulseWidth = pulseIn(
        RADIO_CONTROL_RUDDER_CONTROL_PIN, HIGH);

    if (propSpeedPulseWidth < 1000) {
        radioCommand->propSpeed = -1;
        radioCommand->rudderPosition = 0;
    } else {
        int propSpeed = map(propSpeedPulseWidth, 1000, 2000, -20, 255);
        radioCommand->propSpeed = constrain(propSpeed, 0, 255);

        int rudderPosition = map(rudderPositionPulseWidth, 1000, 2000, -10, 10);
        radioCommand->rudderPosition = constrain(rudderPosition, -10, 10);
    }
}

/*
void pollAndProcessRadioCommands(SailFaceStatus *status) {
    SailFaceRadioCommandMessage radioCommand;
    pollForCurrentRadioCommand(&radioCommand);

    logDebugMessage("RADIO CMD ");
    logDebugMessage(radioCommand.propSpeed);
    logDebugMessage(" ");
    logDebugMessage(radioCommand.rudderPosition);
    logDebugMessage("\n");

    if (radioCommand.propSpeed > 0) {
        prop->setPropellerSpeed(radioCommand.propSpeed, status);
    }
    helm->setRudderPosition(radioCommand.rudderPosition);
}

*/
