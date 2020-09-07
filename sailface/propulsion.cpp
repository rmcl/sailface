#include "sailface.h"
#include "propulsion.h"

void SailFacePropulsion::initialize(SailFaceStatus *status) {

	//  set address if I2C configuration selected with the config jumpers
	motorDriver.settings.commInterface = I2C_MODE;
	motorDriver.settings.I2CAddress = 0x5A; //config pattern "0101" on board for address 0x5A

	// Initialize the driver get wait for idle
	while ( motorDriver.begin() != 0xA9 ) {
		//Wait until a valid ID word is returned
		//Serial.println( "ID mismatch, trying again" );
		delay(500);
	}

	//Serial.println( "ID matches 0xA9" );

	//  Check to make sure the driver is done looking for slaves before beginning
	//Serial.print("Waiting for enumeration...");
	while ( motorDriver.ready() == false );
	//Serial.println("Done.");
	//Serial.println();
}

//void SailFacePropulsion::setSpeed(uint32_t speed)

void SailFacePropulsion::poll(SailFaceStatus *status) {

	Serial.println("start motors!");

	//Use .setDrive( motorNum, direction, level ) to drive the motors.
    motorDriver.setDrive( PRIMARY_PROP_MOTOR, 0, 255);
    //motorDriver.setDrive( 1, 0, 255);
    delay(5000);
    motorDriver.setDrive( PRIMARY_PROP_MOTOR, 0, 0);
    delay(10000);
}

void SailFacePropulsion::writeStatusMessage(SailFaceStatus *status) {

}
