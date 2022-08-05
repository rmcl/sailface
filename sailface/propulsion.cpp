#include "sailface.h"
#include "propulsion.h"

void PropManager::initialize() {
	pinMode(PRIMARY_PROP_PWM_PIN, OUTPUT);

	// Set prop speed to what has been stored in persistent memory.
	setPropellerSpeed(persistedData->getPropSeed());
}

void PropManager::setPropellerSpeed(short newSpeed) {

	if (newSpeed < 0) {
		newSpeed = 0;
	} else if (newSpeed > 255) {
		newSpeed = 255;
	}

	// Don't change the prop speed if the new value is the same as old value.
	if (propSpeed == newSpeed) {
		return;
	}

	analogWrite(PRIMARY_PROP_PWM_PIN, newSpeed);
	propSpeed = newSpeed;

	//Persist the prop speed into persistant memory.
	persistedData->storePropSpeed(propSpeed);
}

short PropManager::getPropellerSpeed() {
	return propSpeed;
}
