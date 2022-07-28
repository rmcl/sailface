#ifndef _BLUETOOTH_COMMAND
#define _BLUETOOTH_COMMAND

#include <Arduino.h>
#include "SerialCommands.h"

#define BLUETOOTH_ENABLE_PIN 9

#define COMM_MAX_COMMAND_LEN 100


/* Commands */
void cmdStatus(SerialCommands* sender) ;

/* Command Calls */
SerialCommand blueCmdStatus("STATUS", cmdStatus);

class BluetoothCommand {

    private:
        bool bluetoothActive;

        char inputBuffer [COMM_MAX_COMMAND_LEN];
        unsigned int inputBufferPosition;
        bool inputBufferReady;

        // Serial2 on the Mega is pins TX2 pin 16 and RX2 pin 17
        // Bluetooth RXD -> TX2 pin 16
        // Bluetooth TXD -> RX2 pin 17
        HardwareSerial &bluetoothSerial = Serial2;


        char serialCommandBuffer[32];
        SerialCommands bluetoothSerialCommands(
            &bluetoothSerial,
            serialCommandBuffer,
            sizeof(serialCommandBuffer),
            "\r\n",
            " "
        );

    public:
        void initialize();

        void pollForBluetoothCommandMessages(

        );
        void sendBluetoothMessage(String *message);
        HardwareSerial *getBluetoothSerial();
};
#endif
