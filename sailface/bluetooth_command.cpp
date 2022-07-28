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

}

void BluetoothCommand::pollForBluetoothCommandMessages() {
    bluetoothSerialCommands.ReadSerial();
}

HardwareSerial *BluetoothCommand::getBluetoothSerial() {
    return &bluetoothSerial;
}


void cmdUnrecognized(SerialCommands* sender, const char* cmd) {
    Serial.println("AHRGGG");
	sender->GetSerial()->print("Unrecognized command [");
	sender->GetSerial()->print(cmd);
	sender->GetSerial()->println("]");
}

void cmdStatus(SerialCommands* sender) {

    PositionInfo curPosition = position->getCurPosition();
    sender->GetSerial()->println(
        String("LAT:") + String(curPosition.latitude) + \
        String(" LONG:") + String(curPosition.longitude) + \
        String(" VALID:") + String(curPosition.positionValid)
    );

    PowerInfo powInfo;
    power->getPowerInfo(&powInfo);
    sender->GetSerial()->println(
        String("BAT VOLT:") + String(powInfo.batteryVoltage) + \
        String(" CURRENT:") + String(powInfo.batteryCurrentDraw)
    );
    sender->GetSerial()->println();

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
