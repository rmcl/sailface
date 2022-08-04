#ifndef _SAILFACE
#define _SAILFACE

#include "position.h"
#include "power.h"
#include "helm.h"
#include "propulsion.h"
#include "navigation.h"
#include "bluetooth_command.h"
#include "iridium.h"

extern PositionManager *position;
extern PowerManager *power;
extern HelmManager *helm;
extern PropManager *prop;
extern NavigationManager *navigation;
extern BluetoothCommand *bluetooth;
extern IridiumManager *iridium;

#endif
