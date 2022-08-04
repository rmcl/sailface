from struct import Struct
from datetime import datetime
import jwt


class SailFaceStatusMessageParser:
    """Parse messages received from SailFace"""

    fields = (
        ('latitude', 'l'),
        ('longitude', 'l'),
        ('waypoint_latitude', 'l'),
        ('waypoint_longitude', 'l'),
        ('battery_voltage', 'h'),
        ('battery_current', 'h'),
        ('prop_speed', 'h')
    )

    def __init__(self):
        """Initialize the message struct parser"""

        # "<" represents little endian
        struct_format_str = '<' + ''.join([
            field[1]
            for field in self.fields
        ])

        self.sailface_struct = Struct(struct_format_str)

    def get_rock7_public_key(self):
        with open('rock7_public.key', 'r') as fp:
            return fp.read()

    def parse_jwt(self, message_jwt):
        """Given a JWT as a string, validate it with Rock7's public key."""
        headers = jwt.get_unverified_header(message_jwt)
        if 'alg' not in headers:
            raise Exception('JWT Algorithm not specified.')

        public_key = self.get_rock7_public_key()
        return jwt.decode(message_jwt, public_key, algorithms=headers['alg'])

    def parse_payload(self, message_data : bytes):
        """Parse the payload of the message a byte string representing the status message struct."""
        if self.sailface_struct.size != len(message_data):
            raise Exception('Message size does not match expected struct size.')

        unpacked_data = self.sailface_struct.unpack(message_data)
        values = {
            self.fields[idx][0]: val
            for idx, val in enumerate(unpacked_data)
        }

        return values

    def parse(self, message : dict):
        """Parse a message received from Rock7.


        Expected message format:
        {
            'JWT': '...',
            'data': '8cda0c026036ddf8bcbdf0ff4a9ff0ff4f000000',
            'device_type': 'ROCKBLOCK',
            'imei': '123232',
            'iridium_cep': 3.0,
            'iridium_latitude': 39.3908,
            'iridium_longitude': -115.7395,
            'iridium_session_status': 0,
            'momsn': 3,
            'serial': 19445,
            'transmit_time': '22-01-15 00:05:43'
        }
        """

        validated_data = self.parse_jwt(message['JWT'])

        transmit_time = datetime.strptime(
            validated_data['transmit_time'],
            '%y-%m-%d %H:%M:%S')
        result = {
            'iridium_latitude': validated_data['iridium_latitude'],
            'iridium_longitude': validated_data['iridium_longitude'],
            'momsn': validated_data['momsn'],
            'transmit_time': transmit_time.isoformat()
        }

        payload_bytes = bytes.fromhex(validated_data['data'])
        payload_values = self.parse_payload(payload_bytes)
        result.update(payload_values)

        return result
