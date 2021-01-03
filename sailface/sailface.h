#ifndef _SAILFACE
#define _SAILFACE

#include <stdint.h>

void logDebugMessage(char *message);

/* Define a data structure that is continuously written to the serial port
for ingestion into the main processor. */
typedef struct {
    // Position
    bool positionValid;
    double latitude;
    double longitude;
    uint32_t gpsFixAge;

    double course; // course in degrees
    double speed; // speed in knots

    // Power Management
    float batteryVoltage;       // voltage in Volts out of the sunny buddy chargin unit.
    float solarPanelCurrent;    // current in mA measured in series with the solar panels
    float batteryCurrentDraw;   // current in mA measured out of the sunny buddy charging unit

    // C&C state
    bool disableMainController;
    bool disablePrimaryPropulsion;


} SailFaceStatus;

#endif
