from unittest import TestCase, Mock
from sailface.wifi_network import WifiNetwork

class WifiNetworkTest(TestCase):
    """Test the WifiNetwork class"""

    def setUp(self):
        self.wlan_mock = Mock()
        self.wlan_mock.scan.return_value = EXAMPLE_WIFI_SCAN_1
        self.wifi = WifiNetwork(wlan=self.wlan_mock)
        

    def test_start(self):
        self.wifi._load_wifi_creds = Mock(return_value=EXAMPLE_WIFI_CREDS_1)
        self.wifi._connect_wifi_network = Mock(return_value=True)
        self.wlan_mock.isconnected.return_value = False
        
        self.wifi.start()

        self.wlan_mock.active.assert_called_once_with(True)
        self.wifi._connect_wifi_network.assert_called_once_with('PartyPalace', 'test')

EXAMPLE_WIFI_SCAN_1 = [
     (b'PartyPalace', b'X\x19', 6, -55, 3, False), 
     (b'', b'\xfa', 6, -59, 0, False), 
     (b'NETGEAR78', b'\x14Y\xc0\xa7}\xf3', 9, -81, 3, False), 
     (b'DIRECT-2A-HP ENVY 7640 series', b'<R\x82 ++', 1, -85, 3, False)
]

EXAMPLE_WIFI_CREDS_1 = [
    {'ssid': 'PartyPalace', 'key': 'test'},
]