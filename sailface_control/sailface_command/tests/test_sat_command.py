from unittest import TestCase
from ctypes import create_string_buffer



from sailface_control.sat_command import (
    SatCommandBuilder,
    Waypoint
)

class SatCommandTest:

    def setUp(self):
        cmd_builder = SatCommandBuilder()

    def test_get_waypoint_struct(self):
        waypoint_struct = cmd_buidler.get_waypoint_struct()
        self.assertEqual(
            waypoint_struct.format,
            'HEELO')

    def test_add_waypoint_to_buffer(self):
        buff = create_string_buffer(500)


        waypoint_1 = Waypoint(
            latitude=119.5,
            longitude=38.2,
            accept_radius=100
        )

        cmd_builder.add_waypoint_to_buffer(
            buff, 0, waypoint_1)