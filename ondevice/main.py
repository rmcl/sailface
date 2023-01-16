from sailface.config import WIFI_CREDS

from sailface.utils import (
    get_i2c_bus,
    setup_sd_card,
    setup_web_repl
)

from sailface.power import PowerMonitor


class SailboardMain:
    """A class to hold some temporary state and run the main loop of the sailboard."""

    def setup(self):
        self.i2c_bus = get_i2c_bus()
        self.sd_card = setup_sd_card()



        self.power = PowerMonitor(self.i2c_bus)


    def loop(self):
        pass


sailboard = SailboardMain()
sailboard.setup()
sailboard.loop()