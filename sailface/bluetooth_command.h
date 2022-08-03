#ifndef _BLUETOOTH_COMMAND
#define _BLUETOOTH_COMMAND

#include <Arduino.h>
#include "SerialCommands.h"

#define BLUETOOTH_ENABLE_PIN 9

#define COMM_MAX_COMMAND_LEN 100

#define SERIAL_COMMANDS_DEBUG true

/* Commands */
void cmdUnrecognized(SerialCommands* sender, const char* cmd);
void cmdStatus(SerialCommands* sender);
void cmdSetRudder(SerialCommands* sender);
void cmdStartProp(SerialCommands* sender);
void cmdStopProp(SerialCommands* sender);
void cmdStartIridium(SerialCommands* sender);
void cmdSleepIridium(SerialCommands* sender);
void cmdPollIridium(SerialCommands* sender);


class BluetoothCommand {

    private:
        bool bluetoothActive;

        // Serial2 on the Mega is pins TX2 pin 16 and RX2 pin 17
        // Bluetooth RXD -> TX2 pin 16
        // Bluetooth TXD -> RX2 pin 17
        HardwareSerial &bluetoothSerial = Serial2;

        char serialCommandBuffer[100];

        SerialCommands bluetoothSerialCommands{
            &bluetoothSerial,
            serialCommandBuffer,
            sizeof(serialCommandBuffer),
            "\n",
            " "
        };

        /* Command Calls */
        SerialCommand blueCmdStatus{"STATUS", cmdStatus};
        SerialCommand blueCmdSetRudder{"RUDDER", cmdSetRudder};
        SerialCommand blueCmdStartProp{"ENGAGE", cmdStartProp};
        SerialCommand blueCmdStopProp{"STOP", cmdStopProp};
        SerialCommand blueCmdStartIridium{"SATSTART", cmdStartIridium};
        SerialCommand blueCmdSleepIridium{"SATSLEEP", cmdSleepIridium};
        SerialCommand blueCmdPollIridium{"SATPOLL", cmdPollIridium};


    public:
        void initialize();

        void pollForBluetoothCommandMessages();
        //void sendBluetoothMessage(String *message);
        bool isBluetoothActive();
        HardwareSerial *getBluetoothSerial();
};
#endif
