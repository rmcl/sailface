# SailFace

An autonomous (non-sail powered) boat thing.


# Code organization

## Microcontroller (sailface)

Runs on the arduino.

## Control (sailface_control)

A Python backed systemd service running on a RaspPI which listens to a Serial-USB port connected to SailFace's arduino based microcontroller. To conserve power the microcontroller can turn-off the RaspPi.


## Communication

The Arduino communicates with the RaspPi and also sends messages to the Iridium Short-Burst Data services via a RockBLOCK 9603.

SailFace status messages:


### Power

```
P:<batteryVoltage>,batteryCurrentDraw,solarPanelCurrent
P:3.70,500,400
```

### Navigation/Position

```
N:<positionValid>,<latitude>,<longitude>,<speed>,<course>,<gpsFixAge>
```
