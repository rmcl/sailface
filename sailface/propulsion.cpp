#include "sailface.h"
#include "propulsion.h"

void PropManager::initialize() {
	pinMode(PRIMARY_PROP_PWM_PIN, OUTPUT);
}

void PropManager::setPropellerSpeed(int newSpeed) {

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
}

int PropManager::getPropellerSpeed() {
	return propSpeed;
}
