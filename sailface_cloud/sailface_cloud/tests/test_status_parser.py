import unittest
from sailface_cloud.status_parser import SailFaceStatusMessageParser

class TestSailFaceStatusMessageParser(unittest.TestCase):

    def setUp(self):
        self.parser = SailFaceStatusMessageParser()

    def test_parse_payload(self):
        """Parse the binary message sent from sailface."""
        payload_bytes = bytes.fromhex(self.EXAMPLE_MESSAGE_1['data'])
        result = self.parser.parse_payload(payload_bytes)

        self.assertEqual(result, {
            'battery_current': 1280,
            'battery_voltage': 98,
            'latitude': 34414836,
            'longitude': -119740664,
            'prop_speed': 0,
            'waypoint_latitude': -1,
            'waypoint_longitude': -1
        })

    def test_jwt_decode(self):
        result = self.parser.parse_jwt(self.EXAMPLE_MESSAGE_1['JWT'])
        self.assertEqual(result, {
            'data': 'f4200d0208e7dcf8ffffffffffffffff620000050000',
            'device_type': 'ROCKBLOCK',
            'iat': 1659636063,
            'imei': '300434064128510',
            'iridium_cep': '73.0',
            'iridium_latitude': '34.4301',
            'iridium_longitude': '-119.3506',
            'iridium_session_status': '2',
            'iss': 'Rock 7',
            'momsn': '4',
            'serial': '19445',
            'transmit_time': '22-08-04 18:00:50'
        })

    def test_parse(self):
        """test that we validate the JWT and decode the message."""
        result = self.parser.parse(self.EXAMPLE_MESSAGE_1)
        self.assertEqual(result, {
            'battery_current': 1280,
            'battery_voltage': 98,
            'iridium_latitude': '34.4301',
            'iridium_longitude': '-119.3506',
            'latitude': 34414836,
            'longitude': -119740664,
            'momsn': '4',
            'prop_speed': 0,
            'transmit_time': '2022-08-04T18:00:50',
            'waypoint_latitude': -1,
            'waypoint_longitude': -1
        })

    EXAMPLE_MESSAGE_1 = { "iridium_session_status":2,"momsn":4,"data":"f4200d0208e7dcf8ffffffffffffffff620000050000","serial":19445,"iridium_latitude":34.4301,"iridium_cep":73.0,"JWT":"eyJhbGciOiJSUzI1NiJ9.eyJpc3MiOiJSb2NrIDciLCJpYXQiOjE2NTk2MzYwNjMsImRhdGEiOiJmNDIwMGQwMjA4ZTdkY2Y4ZmZmZmZmZmZmZmZmZmZmZjYyMDAwMDA1MDAwMCIsImRldmljZV90eXBlIjoiUk9DS0JMT0NLIiwiaW1laSI6IjMwMDQzNDA2NDEyODUxMCIsImlyaWRpdW1fY2VwIjoiNzMuMCIsImlyaWRpdW1fbGF0aXR1ZGUiOiIzNC40MzAxIiwiaXJpZGl1bV9sb25naXR1ZGUiOiItMTE5LjM1MDYiLCJpcmlkaXVtX3Nlc3Npb25fc3RhdHVzIjoiMiIsIm1vbXNuIjoiNCIsInNlcmlhbCI6IjE5NDQ1IiwidHJhbnNtaXRfdGltZSI6IjIyLTA4LTA0IDE4OjAwOjUwIn0.d2EsUay48-eEVN8V-bpY724sbkEmCmGRx4dNrHA6o5ch2puNBrC3wjcalnUnt7nwuT6A5Wa12aMuzBjTFAbXkaGsJr0o95Kv8xBgr69yjCKIIuPiX1Gfc776BIygY7nlns40maIKLHfWcQZvh_0K1m6JdEcVG7K1ZClV3iEHw8ENQC6wZRMGB-JmLGRZeJ0R7F3sH9ruuglEsKHn83BxXSwxAHZpX1PjZf4kcJn29-kkZbKzRCXnke96wWB9VgHtR5F4hqQnDJKflVPQ67fvM9ylKY3KiicmEN97FHC0KPCQllAEdmuwAzv8hnFQ_q9vuO1KPtvxXfZPRHJUUcG1FA","imei":"300434064128510","device_type":"ROCKBLOCK","transmit_time":"22-08-04 18:00:50","iridium_longitude":-119.3506}
