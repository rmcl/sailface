# SailFace

An autonomous (non-sail powered) boat thing.


# Code organization

## Microcontroller (sailface)

Runs on an arduino MEGA. This code manages navigation, communication and power management.

## Control (sailface_control)

Python code for interacting with the microcontroller via serial - wired or bluetooth as well as decoding iridium API messages.

## Cloud (cloud)

Lambda functions for receiving API callbacks from rock7 and storing them in a s3 bucket.

## Schematics

PCB/Schematic Designs for an Arduino shield which connects the various electronic components. Created using KiCAD

## 3dmodels

3d models for various parts largely for supporting the internals of the vessel.


# Communication

The Arduino communicates sends messages to the Iridium Short-Burst Data services via a RockBLOCK 9603.

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
