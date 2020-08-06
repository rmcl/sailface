# SailFace

An autonomous (non-sail powered) boat thing.



## Communication

The Arduino communicates with the RaspPi and also sends messages to the Iridium Short-Burst Data services via a RockBLOCK 9603.

SailFace status messages:


### Power

```
P:<batteryVoltage>,batteryCurrentDraw
P:3.70,500
```

### Navigation/Position

```
N:<positionValid>,<latitude>,<longitude>,<speed>,<course>,<gpsFixAge>
```
