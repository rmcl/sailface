#ifndef _SAILFACE
#define _SAILFACE

#include <stdint.h>


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
    int batteryVoltage;
    int instantaneousCurrentDraw;

} SailFaceStatus;

#endif
