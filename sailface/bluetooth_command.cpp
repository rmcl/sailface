void BluetoothCommand::initialize() {

    bluetoothActive = true;

    // Turn on bluetooth module
    pinMode(BLUETOOTH_ENABLE_PIN, OUTPUT);
    digitalWrite(BLUETOOTH_ENABLE_PIN, HIGH);

    delay(1000);

    bluetoothSerial.begin(9600);

    bluetoothSerialCommands.SetDefaultHandler(cmdUnrecognized);
	bluetoothSerialCommands.AddCommand(&blueCmdStatus);
}

void BluetoothCommand::pollForBluetoothCommandMessages() {
    bluetoothSerialCommands.ReadSerial();
}


void cmdUnrecognized(SerialCommands* sender, const char* cmd)
{
	sender->GetSerial()->print("Unrecognized command [");
	sender->GetSerial()->print(cmd);
	sender->GetSerial()->println("]");
}

void cmdStatus(SerialCommands* sender) {

    //bluetooth->println(curPosition.positionValid);
    sender->GetSerial()->print("HEAD: " + String(curPosition.magneticHeading, 5));
    sender->GetSerial()->println(
        " LAT: " + String(curPosition.latitude, 5) + " LONG: " + String(curPosition.longitude, 5)
    );

    PowerInfo powInfo;
    power->getPowerInfo(&powInfo);
    sender->GetSerial()->println(
        "BAT VOLT:" + String(powInfo.batteryVoltage) + " Current:" + String(powInfo.batteryCurrentDraw)
    );
}
