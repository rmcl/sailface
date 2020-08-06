# SailFace

An autonomous (non-sail powered) boat thing.


## Control (sailface_control)

A Python backed systemd service running on a RaspPI which listens to a Serial-USB port connected to SailFace's arduino based microcontroller. To conserve power the microcontroller can turn-off the RaspPi.
