import network
import webrepl
import json
import time


class WifiNetwork:
    """Manage the wifi connection when sailface is within range."""

    def __init__(self, wlan = None):
        if wlan:
            self.wlan = wlan
        else:
            self.wlan = network.WLAN(network.STA_IF)

    def on_connect(self):
        """Setup the webrepl for remote access to the micropython REPL."""
        webrepl.start()

    def start(self):
        """Attempt to connect to all known wifi network.
        
        * Load credentials from SD card or filesystem
        * Scan for available networks
        * Attempt to connect to each network we have creds for
            in the order they are listed in the creds file
        * If we successfully connect to a network, call on_connect
        """
        self.wlan.active(True)

        if self.wlan.isconnected():
            return True

        creds = self._load_wifi_creds()

        available_network_details = self.wlan.scan()
        available_ssids = []
        for network_detail in available_network_details:
            ssid = network_detail[0].decode('utf-8').strip()
            if ssid:        
                available_ssids.append(ssid)

        for cred in creds:
            if cred['ssid'] in available_ssids:
                print(cred['ssid'])
                if self._connect_wifi_network(cred['ssid'], cred['key']):
                    self.on_connect()
                    return True
        
        return False

    def stop(self):
        webrepl.stop()
        self.wlan.active(False)

    def _connect_wifi_network(self, ssid, key):
        """Attempt to connect to local wifi network."""    

        print(f'connecting to network... {ssid}')
        self.wlan.connect(ssid, key)
        start_time = time.time()
        while not self.wlan.isconnected():

            # Spend no more than 30 seconds trying to connect
            if start_time + 30 < time.time():
                print('failed. timed out')
                return False

        print('network config:', self.wlan.ifconfig())
        return True


    def _load_wifi_creds(self):
        try:
            with open('/sd/wifi_creds.json', 'r') as fp:
                return json.load(fp)
        except OSError:
            pass

        # If SD card is not mounted then as a fallback, 
        # load the wifi creds from the onboard flash memory.
        with open('fallback_creds.json', 'r') as fp:
            return json.load(fp)
