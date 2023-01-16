"""Constants for the Sailface Board."""

# I2C Configuration
I2C_SCL_PIN = 22
I2C_SDA_PIN = 21


# SDCard Configuration
# The sailboard utilizes the pin configuration specified in the micropython on
# the esp32 documentation for "slot 1". 
# https://docs.micropython.org/en/latest/library/machine.SDCard.html
SDCARD_SLOT = 1
SDCARD_MOUNT_POINT = '/sd'