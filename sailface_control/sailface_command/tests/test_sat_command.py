from unittest import TestCase
from ctypes import create_string_buffer


from sailface_command.sat_command import (
    SatCommandBuilder,
    Waypoint
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