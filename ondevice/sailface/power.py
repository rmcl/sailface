import sys
from machine import SoftI2C

from ina219 import INA219
from logging import INFO, DEBUG
from .logs import get_logger
from .exceptions import PowerMonitorBadReading

POWER_MAIN_I2C_ADDRESS = 0x40
POWER_SOLAR_SHUNT_OHMS = 0.11

POWER_SOLAR_I2C_ADDRESS = 0x41
POWER_SOLAR_SHUNT_OHMS = 0.11

BAD_READING_ALARM_COUNT = 5

class PowerMonitor:
    """A class to monitor the power usage of the sailface board.
    
    The Sailboard is equipped with two INA219 power monitors. One is
    connected to the main battery supply and the other is connected 
    between the solar panel and the battery.
    """

    def __init__(self, i2c_bus : SoftI2C):
        """Initialize the power monitors."""

        self.logger = get_logger('power-monitor')

        self.mainpower_ready = False
        self.solarpower_ready = False

        self.configure_ina219(i2c_bus)

        self.bad_reading_count = 0

    def configure_ina219(self, i2c_bus : SoftI2C):

        self.mainpower_ina = INA219(
            POWER_SOLAR_SHUNT_OHMS,
            i2c_bus,
            address=POWER_MAIN_I2C_ADDRESS)

        try:
            self.mainpower_ina.configure()
            self.mainpower_ready = True
        except OSError as error:
            sys.print_exception(error)
            self.logger.error("Failed to configure main power monitor" + str(error))

        self.solar_ina = INA219(
            POWER_SOLAR_SHUNT_OHMS,
            i2c_bus,
            address=POWER_SOLAR_I2C_ADDRESS)

        try:
            self.solar_ina.configure()
            self.solarpower_ready = True
        except OSError as error:
            self.logger.error("Failed to configure solar power monitor" + str(error))

    def poll(self) -> dict:
        """Poll the power monitor and update the values"""

        if not (self.mainpower_ready and self.solarpower_ready):
            self.logger.error("Power monitors not ready")
            return {}

        main_current = self.mainpower_ina.current()
        if main_current <= 0.01:
            self.bad_reading_count += 1

            if self.bad_reading_count > 2:
                self.logger.error("Bad reading from main power monitor")
                self.configure_ina219(self.mainpower_ina.i2c)

            if self.bad_reading_count > BAD_READING_ALARM_COUNT:
                self.logger.error("Bad reading from main power monitor")
                self.bad_reading_count = 0
                raise PowerMonitorBadReading('Bad reading from main power monitor')
        else:
            self.bad_reading_count = 0


        return {
            "main_voltage": self.mainpower_ina.voltage(),
            "main_current": main_current,
            "main_power": self.mainpower_ina.power(),

            "solar_voltage": self.solar_ina.voltage(),
            "solar_current": self.solar_ina.current(),
            "solar_power": self.solar_ina.power(),
        }

    