# Add the vendor directory to the path so we can import additional libraries
import sys
sys.path.append('/vendor')

import json
import logging
import machine
from machine import Timer

from sailface.utils import (
    get_i2c_bus,
    setup_sd_card,
    get_exception_string
)

from sailface.exceptions import PowerMonitorBadReading
from sailface.wifi_network import WifiNetwork
from sailface.prop import MainPropControl
from sailface.power import PowerMonitor
from sailface.logs import (
    get_logger,
    setup_sd_log_handler,
    setup_telemetry_api_log_handler
)

logger = get_logger('sailface-main')


class SailboardMain:
    """A class to hold some temporary state and run the main loop of the sailboard."""

    def setup(self):
        self.i2c_bus = get_i2c_bus()

        try:
            self.sd_card = setup_sd_card()
            setup_sd_log_handler()
        except OSError as error:
            logger.error('Failed to setup sd card: ' + get_exception_string(error))

        self.power = PowerMonitor(self.i2c_bus)
        self.wifi = WifiNetwork()
        if not self.wifi.start():
            logger.error('Failed to connect to wifi network')
            raise Exception('Failed to connect to wifi network')
        else:
            setup_telemetry_api_log_handler()

        self.power_logger = get_logger('power')
        self.prop_control = MainPropControl()

        # disable prop for now
        #self.prop_control.forward(50)
        self.prop_control.stop()

        self.reset_timer = Timer(-1)
        self.reset_timer.init(mode=Timer.ONE_SHOT, period=1000 * 60 * 60, callback=self.reset_callback)

        logger.info('Boot setup complete')

    def reset_callback(self, timer : Timer):
        """Once an hour, reset the machine to try to resolve any issues."""
        logger.info('Resetting machine due to hourly reset timer')
        machine.reset()

    def schedule_next_run(self, time_ms):
        timer = Timer(-1)
        timer.init(mode=Timer.ONE_SHOT, period=time_ms, callback=self.timer_callback)

    def timer_callback(self, timer : Timer):
        self.loop()

    def loop(self):
        self.poll_power_and_log()

        # 30 seconds
        self.schedule_next_run(1000 * 30)

    def poll_power_and_log(self):
        try:
            power_details = self.power.poll()
            self.power_logger.info(json.dumps(power_details))

        except PowerMonitorBadReading:
            logger.error('Bad reading from power monitor')

            # let's reset the machine to try to resolve the issue
            machine.reset()

        except Exception as error:
            logger.error('An unexpected error occurred')
            logger.error('Unexpected error: ' + str(error))