import os
import sys
import json
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

def load_config():
    """Load the config file from the SD card. If SD not available then load backup from flash."""
    try:
        with open('/sd/config.json', 'r') as fp:
            return json.load(fp)
    except OSError:
        pass

    # If SD card is not mounted then as a fallback,
    # load the wifi creds from the onboard flash memory.
    with open('fallback_config.json', 'r') as fp:
        return json.load(fp)
