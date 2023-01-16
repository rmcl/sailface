import os
from machine import (
    Pin,
    SoftI2C,
    SDCard
)

from .constants import (
    I2C_SCL_PIN, I2C_SDA_PIN,
    SDCARD_MOUNT_POINT, SDCARD_SLOT
)

def get_i2c_bus() -> SoftI2C:
    """Get the I2C bus for the ESP32 using the sailboard defined pins."""
    return SoftI2C(
        1,
        scl=Pin(I2C_SCL_PIN),
        sda=Pin(I2C_SDA_PIN),
        freq=400000)


def setup_sd_card() -> SDCard:
    """Initialize the SD card and mount it to the micropython filesystem."""
    sd = SDCard(slot=SDCARD_SLOT)
    os.mount(sd, SDCARD_MOUNT_POINT)
    return sd

