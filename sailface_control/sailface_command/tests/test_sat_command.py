from unittest import TestCase
from ctypes import create_string_buffer


from sailface_command.sat_command import (
    SatCommandBuilder,
    SailFaceCommand,
    Waypoint,

    WAYPOINT_OVERWRITE
)

class SatCommandTest(TestCase):
    """Test the satellite command builder."""
    def setUp(self):
        self.cmd_builder = SatCommandBuilder()

    def test_get_waypoint_struct(self):
        waypoint_struct = self.cmd_builder.get_waypoint_struct()
        self.assertEqual(
            waypoint_struct.format,
            '<lll')

    def test_add_waypoint_to_buffer(self):
        buff = create_string_buffer(500)

        waypoint_1 = Waypoint(
            latitude=119.542,
            longitude=38.255,
            accept_radius=100
        )

        new_offset = self.cmd_builder.add_waypoint_to_buffer(
            buff, 0, waypoint_1)

        # three floats, 4 bytes each = 12 bytes
        self.assertEqual(new_offset, 12)

        waypoint_struct = self.cmd_builder.get_waypoint_struct()
        parsed_waypoint = waypoint_struct.unpack_from(buff,0)

        self.assertEqual(waypoint_1.latitude, round(parsed_waypoint[0] * 1e-6, 6))
        self.assertEqual(waypoint_1.longitude, round(parsed_waypoint[1] * 1e-6, 6))
        self.assertEqual(waypoint_1.accept_radius, parsed_waypoint[2])

    def test_add_command_message_to_buffer(self):
        buff = create_string_buffer(500)
        cmd_1 = self.mk_command_message()

        res_len = self.cmd_builder.add_command_message_to_buffer(buff, cmd_1)

        self.assertEqual(res_len, 37)

    def test_get_command(self):
        """"""
        cmd_1 = self.mk_command_message()
        result = self.cmd_builder.get_command(cmd_1)
        self.assertEqual(
            result,
            '960001003c0000000101000200f010200798b9470264000000709e7b07b8c467024b000000')



    def mk_command_message(self):
        waypoint_1 = Waypoint(
            latitude=119.542,
            longitude=38.255,
            accept_radius=100
        )
        waypoint_2 = Waypoint(
            latitude=125.542,
            longitude=40.355,
            accept_radius=75
        )

        return SailFaceCommand(
            prop_speed=150,
            bluetooth_active=True,
            update_frequency_minutes=60,
            navigate_to_waypoint=True,
            waypoint_action=WAYPOINT_OVERWRITE,
            waypoints=[
                waypoint_1,
                waypoint_2
            ]
        )