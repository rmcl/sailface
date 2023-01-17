import os
import sys
from io import StringIO
from machine import (
    Pin,
    I2C,
    SDCard
)
#from .exceptions import SDCardException
from .constants import (
    I2C_SCL_PIN, I2C_SDA_PIN,
    SDCARD_MOUNT_POINT, SDCARD_SLOT
)

def get_i2c_bus() -> I2C:
    """Get the I2C bus for the ESP32 using the sailboard defined pins."""
    return I2C(
        1,
        scl=Pin(I2C_SCL_PIN),
        sda=Pin(I2C_SDA_PIN))


def setup_sd_card() -> SDCard:
    """Initialize the SD card and mount it to the micropython filesystem."""
    #try:
    sd = SDCard(slot=SDCARD_SLOT)
    os.mount(sd, SDCARD_MOUNT_POINT)
    #except OSError as error:
    #print('Failed to mount SD card')
    #raise SDCardException('Failed to mount SD card')

    return sd

def get_exception_string(error):
    """Return the current exception as a string so it can be logged."""
    tmp_error_buf = StringIO()
    sys.print_exception(error, tmp_error_buf)
    tmp_error_buf.seek(0)
    return tmp_error_buf.read()