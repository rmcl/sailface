# SailFace

An autonomous (non-sail powered) boat thing.

# Photos

## Mark II

## Mark I




# Code organization

## Microcontroller (sailface)

Runs on an arduino MEGA. This code manages navigation, communication and power management.

## Control (sailface_control)

Python code for controling sailface remotely.

### Lambda Data Capture (lambda_data_capture)

This module is a lambda function that will receive the messages relayed from the Iridium Short-Burst Data (SBD) network via the RockBLOCK service (https://docs.rockblock.rock7.com/docs/getting-started). 

### Command Lib (sailface_command)

A python library for creating command messages that can be sent to the vessel.

### Notebooks

Various jupyter notebooks for processing KML + sending commands to the vessel.

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
