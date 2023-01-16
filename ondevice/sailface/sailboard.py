# Add the vendor directory to the path so we can import additional libraries
import sys
sys.path.append('/vendor')

import json
import logging
from machine import Timer

from sailface.utils import (
    get_i2c_bus,
    setup_sd_card
)

from sailface.exceptions import SDCardException
from sailface.wifi_network import WifiNetwork
from sailface.power import PowerMonitor
from sailface.logs import get_logger, setup_sd_log_handler

logger = get_logger('sailface-main')


class SailboardMain:
    """A class to hold some temporary state and run the main loop of the sailboard."""

    def setup(self):
        self.i2c_bus = get_i2c_bus()

        try:
            self.sd_card = setup_sd_card()
            setup_sd_log_handler()
        except SDCardException:
            pass            

        self.power = PowerMonitor(self.i2c_bus)
        self.wifi = WifiNetwork()
        if not self.wifi.start():
            logger.error('Failed to connect to wifi network')
            raise Exception('Failed to connect to wifi network')

        self.power_logger = get_logger('power')


    def schedule_next_run(self, time_ms):
        timer = Timer(-1)
        timer.init(mode=Timer.ONE_SHOT, period=time_ms, callback=self.timer_callback)

    def timer_callback(self, timer : Timer):
        self.loop()

    def loop(self):
        print('LOOP CALLED!')

        self.poll_power_and_log()
        
        self.schedule_next_run(10000)

    def poll_power_and_log(self):
        power_details = self.power.poll()        
        self.power_logger.info(json.dumps(power_details))
