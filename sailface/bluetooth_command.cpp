#include "bluetooth_command.h"

#include "sailface.h"


void BluetoothCommand::initialize() {

    // Retrieve if Bluetooth is active from EEPROM data
    bluetoothActive = persistedData->getBluetoothAcive();

    // Turn on bluetooth module
    pinMode(BLUETOOTH_ENABLE_PIN, OUTPUT);
    digitalWrite(BLUETOOTH_ENABLE_PIN, HIGH);

    delay(1000);

    bluetoothSerial.begin(9600);

    bluetoothSerialCommands.SetDefaultHandler(cmdUnrecognized);
    bluetoothSerialCommands.AddCommand(&blueCmdHelp);
    bluetoothSerialCommands.AddCommand(&blueCmdStatus);
    bluetoothSerialCommands.AddCommand(&blueCmdSetRudder);
    bluetoothSerialCommands.AddCommand(&blueCmdStartProp);
    bluetoothSerialCommands.AddCommand(&blueCmdStopProp);
    bluetoothSerialCommands.AddCommand(&blueCmdStartIridium);
    bluetoothSerialCommands.AddCommand(&blueCmdSleepIridium);
    bluetoothSerialCommands.AddCommand(&blueCmdPollIridium);
    bluetoothSerialCommands.AddCommand(&blueCmdCalibrateMPU);

    bluetoothSerialCommands.AddCommand(&blueCmdStartNavWaypoint);
    bluetoothSerialCommands.AddCommand(&blueCmdStopNavWaypoint);
    bluetoothSerialCommands.AddCommand(&blueCmdSetPIDParams);

    bluetoothSerialCommands.AddCommand(&blueCmdWaypointAppend);
}

bool BluetoothCommand::isBluetoothActive() {
    return bluetoothActive;
}

void BluetoothCommand::pollForBluetoothCommandMessages() {
    bluetoothSerialCommands.ReadSerial();

    // wait for send buffer to empty before proceeding.
    //bluetoothSerial.flush();
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

void cmdHelp(SerialCommands* sender) {
    sender->GetSerial()->println(
        String("SAILFACE BLUETOOTH COMMANDS\n") + \
        String("  status: return status of the craft\n") + \
        String("  rudder <int:-10-10>: set the angle of the rudder\n") + \
        String("  engage <int:0-255: Set the speed of the main prop.\n") + \
        String("  satstart: Start the iridium sat radio.\n") + \
        String("  satsleep: Sleep the iridium sat radio.\n") + \
        String("  satpoll: Force a poll for sat messages.\n") + \
        String("  wpa <lat> <lon> <accept radius>: Append a waypoint.\n") + \
        String("  navstart: start navigating towards the active waypoint.\n") + \
        String("  navstop: stop navigating towards the active waypoint.\n")

    );
}

void cmdStatus(SerialCommands* sender) {

    PositionInfo curPosition = position->getCurPosition();
    sender->GetSerial()->println(
        String("POSITION\n") + \
        String(" LAT:") + String(curPosition.latitude) + String("\n") + \
        String(" LONG:") + String(curPosition.longitude) + String("\n") + \
        String(" VAL:") + String(curPosition.positionValid) + String("\n") + \
        String(" MPU:") + String(curPosition.magneticHeading) + String("\n") + \
        String(" VAR:") + String(curPosition.magneticHeadingVariation)
    );

    PowerInfo powInfo;
    power->getPowerInfo(&powInfo);
    sender->GetSerial()->println(
        String("Power:\n") + \
        String(" BAT VOLT:") + String(powInfo.batteryVoltage) + String("\n") + \
        String(" CURRENT:") + String(powInfo.batteryCurrentDraw)
    );

    sender->GetSerial()->println(
        String("Propulsion & Navigation:\n") + \
        String(" PROP SPEED:") + String(prop->getPropellerSpeed()) + String("\n")
    );

    PIDParams params = helm->getPIDParams();
    sender->GetSerial()->println(
        String(" NAV TO WAYPOINT:") + String(navigation->isNavigatingToWaypoint()) + String("\n") + \
        String("Kp: ") + String(params.Kp) + String(" Ki: ") + String(params.Ki) + String(" Kd: ") + String(params.Kd) + String("\n")
    );    

    Waypoint activeWaypoint = navigation->getActiveWaypoint();
    //if (activeWaypoint != NULL) {
        sender->GetSerial()->println(
            String("ACTIVE WP:\n") + \
            String(" LAT:") + String(activeWaypoint.latitude) + \
            String(" LONG:") + String(activeWaypoint.longitude) + \
            String(" RADIUS:") + String(activeWaypoint.allowedRadiusMeters) + String("\n")
        );
    //}

    sender->GetSerial()->println(
        String("SAT\n LAST TRANSMIT:") + String(iridium->getLastTransmitTime()) + String("\n") + \
        String(" ACTIVE: ") + String(iridium->isIridiumActive()) + String("\n") + \
        String(" BUSY/TX IN PROG: ") + String(iridium->isIridiumBusy()) + String("\n") + \
        String(" TRANSMIT FREQUENCY:") + String(iridium->getUpdateFrequency())
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
    //Expect one argument from 0-255
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
        String(iridium->isIridiumActive()) + \
        String("BUSY:" ) + \
        String(iridium->isIridiumBusy())
    );
    int messageCount = iridium->pollForCommandMessages(true);
    sender->GetSerial()->println(
        String("Num MSG: ") + \
        String(messageCount)
    );
}

void cmdCalibrationMPU(SerialCommands* sender) {
    //MPUCalibrationParams params = position->calibrateMPU(sender->GetSerial());
    //position->printMPUCalibrationSettings(&params, sender->GetSerial());
}

void cmdStartNavWaypoint(SerialCommands* sender) {
    sender->GetSerial()->println("INFO: START NAVIGATING TO WAYPOINT");
    navigation->startNavigatingToWaypoint();
}

void cmdStopNavWaypoint(SerialCommands* sender) {
    sender->GetSerial()->println("INFO: STOP NAVIGATING TO WAYPOINT");
    navigation->stopNavigatingToWaypoint();
}

void cmdWaypointAppend(SerialCommands* sender) {
    sender->GetSerial()->println("INFO: APPEND WAYPOINT");
    char* longitudeStr = sender->Next();
    char* latitudeStr = sender->Next();
    char* acceptRadiusStr = sender->Next();
    
    Waypoint newWaypoint;
    newWaypoint.longitude = atol(longitudeStr);
    newWaypoint.latitude = atol(latitudeStr);
    newWaypoint.allowedRadiusMeters = atol(acceptRadiusStr);

    navigation->updateWaypoints(&newWaypoint, 1, true);

}

void cmdSetPIDParams(SerialCommands* sender) {
    sender->GetSerial()->println("INFO: SET PID PARAMS");
    char* kpStr = sender->Next();
    char* kiStr = sender->Next();
    char* kdStr = sender->Next();

    PIDParams newParams;
    newParams.Kp = (double)atof(kpStr);
    newParams.Ki = (double)atof(kiStr);
    newParams.Kd = (double)atof(kdStr);

    helm->setPIDParams(newParams);
}