#include "sailface.h"
#include "propulsion.h"

void SailFacePropulsion::initialize(SailFaceStatus *status) {

	Serial.println(";--Prop World!--");

	//  set address if I2C configuration selected with the config jumpers
	motorDriver.settings.commInterface = I2C_MODE;
	motorDriver.settings.I2CAddress = 0x5A; //config pattern "0101" on board for address 0x5A

	// Initialize the driver get wait for idle
	while ( motorDriver.begin() != 0xA9 ) {
	  //Wait until a valid ID word is returned
	  Serial.println( "ID mismatch, trying again" );
	  delay(500);
	}

	//  Check to make sure the driver is done looking for slaves before beginning
	Serial.println("Waiting for enumeration...");
	while ( motorDriver.ready() == false );

	while ( motorDriver.busy() );
	motorDriver.enable();

	Serial.println("Propulsion initialized.");
}

void SailFacePropulsion::setPropellerSpeed(int speed, SailFaceStatus *status) {
	if (speed < 0) {
		speed = 0;
	} else if (speed > 255) {
		speed = 255;
	}

	motorDriver.setDrive(PRIMARY_PROP_MOTOR, 0, speed);

	status->propSpeed = speed;
}
