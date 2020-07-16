#include "sailface.h"

#include "position.h"

SailFaceStatus globalStatus;
SailFacePositionManagement *positionManager;

void setup(void) {
    Serial.begin(115200);
    Serial.println(";--Hello World!--");

    positionManager = new SailFacePositionManagement();

    positionManager->initialize(&globalStatus);

    while (!Serial) {
        delay(1);
    }
}

void writeStatusToSerial(SailFaceStatus *status) {
    Serial.print(status->positionValid);
    Serial.print(",");
    Serial.print(status->latitude, 6);
    Serial.print(",");
    Serial.print(status->longitude, 6);
    Serial.print(",");
    Serial.print(status->speed);
    Serial.print(",");
    Serial.print(status->course);
    Serial.print(",");
    Serial.print(status->gpsFixAge);
    Serial.print("\n");
}

void writeStatusToSerialHumanReadable(SailFaceStatus *status) {
    Serial.println(";--Position--");
    Serial.print(";position is valid:"); Serial.print(status->positionValid); Serial.println("");
    Serial.print(";speed:"); Serial.print(status->speed); Serial.println("");
    Serial.print(";course:"); Serial.print(status->course); Serial.println("");
    Serial.print(";lat:"); Serial.print(status->latitude, 6); Serial.println("");
    Serial.print(";long:"); Serial.print(status->longitude, 6); Serial.println("");
    Serial.print(";age:"); Serial.print(status->gpsFixAge); Serial.println("");
    Serial.println(";---");
}

void loop(void) {
    Serial.println(";---Start Loop---");

    // Call each module
    positionManager->poll(&globalStatus);

    writeStatusToSerial(&globalStatus);
    Serial.println(";---End Loop---");

    delay(1000);
}
