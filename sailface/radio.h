#ifndef _SAIL_FACE_RADIO
#define _SAIL_FACE_RADIO

#include <stdint.h>
#include <Arduino.h>

/* Radio Control Related */

#define RADIO_CONTROL_PROP_SPEED_PIN 11
#define RADIO_CONTROL_RUDDER_CONTROL_PIN 10

// Structure of messages received from the RC controller.
typedef struct {
  // speed of prop between 0 and 255. Set to -1 for no change.
  int propSpeed;
  int rudderPosition;

} SailFaceRadioCommandMessage;


class RadioManager {

    private:
        bool radioControlActive;

    public:
        void initialize();
        void startRadioControl();
        void stopRadioControl();
        void pollForCurrentRadioCommand(SailFaceRadioCommandMessage *radioCommand);

};
#endif
