#include "bluetooth_command.h"

#include "sailface.h"


void BluetoothCommand::initialize() {

    bluetoothActive = true;

    // Turn on bluetooth module
    pinMode(BLUETOOTH_ENABLE_PIN, OUTPUT);
    digitalWrite(BLUETOOTH_ENABLE_PIN, HIGH);

    delay(1000);

    bluetoothSerial.begin(57600);

    bluetoothSerialCommands.SetDefaultHandler(cmdUnrecognized);
	bluetoothSerialCommands.AddCommand(&blueCmdStatus);
    bluetoothSerialCommands.AddCommand(&blueCmdSetRudder);
    bluetoothSerialCommands.AddCommand(&blueCmdStartProp);
    bluetoothSerialCommands.AddCommand(&blueCmdStopProp);
    bluetoothSerialCommands.AddCommand(&blueCmdStartIridium);
    bluetoothSerialCommands.AddCommand(&blueCmdSleepIridium);
    bluetoothSerialCommands.AddCommand(&blueCmdPollIridium);
}

bool BluetoothCommand::isBluetoothActive() {
    return bluetoothActive;
}

void BluetoothCommand::pollForBluetoothCommandMessages() {
    bluetoothSerialCommands.ReadSerial();

    // wait for send buffer to empty before proceeding.
    bluetoothSerial.flush();
}

HardwareSerial *BluetoothCommand::getBluetoothSerial() {
    return &bluetoothSerial;
}


void cmdUnrecognized(SerialCommands* sender, const char* cmd) {
    Stream *serial = sender->GetSerial();
    serial->print("Unrecognized command [");
	serial->print(cmd);
	serial->println("]");
    serial->flush();
}

void cmdStatus(SerialCommands* sender) {

    PositionInfo curPosition = position->getCurPosition();
    sender->GetSerial()->println(
        String("LAT:") + String(curPosition.latitude) + \
        String(" LONG:") + String(curPosition.longitude) + \
        String(" VAL:") + String(curPosition.positionValid) + \
        String(" MPU:") + String(curPosition.magneticHeading) + \
        String(" VAR:") + String(curPosition.magneticHeadingVariation)
    );

    PowerInfo powInfo;
    power->getPowerInfo(&powInfo);
    sender->GetSerial()->println(
        String("BAT VOLT:") + String(powInfo.batteryVoltage) + \
        String(" CURRENT:") + String(powInfo.batteryCurrentDraw)
    );
    sender->GetSerial()->println();
    sender->GetSerial()->flush();

}

void cmdSetRudder(SerialCommands* sender) {

    // Expect one argument of type int specifying the rudder position from -10->+10
    char* rudderPositionStr = sender->Next();
	if (rudderPositionStr == NULL) {
		sender->GetSerial()->println("ERROR: Rudder position not specified");
		return;
	}

	int rudderPosition = atoi(rudderPositionStr);
    helm->setRudderPosition(rudderPosition);

    sender->GetSerial()->println(
        "INFO: Set rudder position: " + String(rudderPosition));
}

void cmdStartProp(SerialCommands* sender) {
    // Expect one argument of type int specifying the rudder position from -10->+10
    char* propSpeedStr = sender->Next();
	if (propSpeedStr == NULL) {
		sender->GetSerial()->println("ERROR: Prop speed not specified");
		return;
	}
	int propLevel = atoi(propSpeedStr);

    sender->GetSerial()->println("INFO: Set prop speed: " + String(propLevel));
    prop->setPropellerSpeed(propLevel);
}

void cmdStopProp(SerialCommands* sender) {
    sender->GetSerial()->println("INFO: Set prop speed: 0");
    prop->setPropellerSpeed(0);
}

void cmdStartIridium(SerialCommands* sender) {
    iridium->wakeIridium();
}
void cmdSleepIridium(SerialCommands* sender) {
    iridium->sleepIridium();
}

void cmdPollIridium(SerialCommands* sender) {
    sender->GetSerial()->println("INFO: IRIDIUM POLL");
    sender->GetSerial()->println(
        String("ACTIVE:" ) + \
        String(iridium->isIridiumActive())
    );
    int messageCount = iridium->pollForCommandMessages(true);
    sender->GetSerial()->println(
        String("Num MSG: ") + \
        String(messageCount)
    );
}
