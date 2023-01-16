from machine import SoftI2C
from ina219 import INA219
from logging import INFO

POWER_MAIN_I2C_ADDRESS = 0x40
POWER_SOLAR_SHUNT_OHMS = 0.15

POWER_SOLAR_I2C_ADDRESS = 0x41
POWER_SOLAR_SHUNT_OHMS = 0.15


class PowerMonitor:
    """A class to monitor the power usage of the sailface board.
    
    The Sailboard is equipped with two INA219 power monitors. One is
    connected to the main battery supply and the other is connected 
    between the solar panel and the battery.
    """

    def __init__(self, i2c_bus : SoftI2C):
        """Initialize the power monitors."""
        self.mainpower_ina = INA219(
            POWER_SOLAR_SHUNT_OHMS,
            i2c_bus,
            log_level=INFO,
            address=POWER_MAIN_I2C_ADDRESS)
        self.mainpower_ina.configure()

        self.solar_ina = INA219(
            POWER_SOLAR_SHUNT_OHMS,
            i2c_bus,
            log_level=INFO,
            address=POWER_SOLAR_I2C_ADDRESS)
        self.solar_ina.configure()

    def poll(self) -> dict:
        """Poll the power monitor and update the values"""
        return {
            "main_voltage": self.mainpower_ina.voltage(),
            "main_current": self.mainpower_ina.current(),
            "main_power": self.mainpower_ina.power(),
            
            "solar_voltage": self.solar_ina.voltage(),
            "solar_current": self.solar_ina.current(),
            "solar_power": self.solar_ina.power(),
        }

    