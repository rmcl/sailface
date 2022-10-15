from typing import List
from struct import Struct
from ctypes import create_string_buffer, Array
from dataclasses import dataclass


@dataclass
class Waypoint:
    latitude: float
    longitude: float
    accept_radius: float

@dataclass
class SailFaceCommand:
    prop_speed: int
    bluetooth_active: bool
    update_frequency_minutes: int
    navigate_to_waypoint: bool
    waypoint_action: int
    waypoints: List[Waypoint]


class SatCommandBuilder:
    def add_waypoint_to_buffer(
        self,
        buff : Array,
        buff_offset : int,
        waypoint_details : Waypoint
    ):
        """Build a waypoint structure."""
        waypoint_struct = self.get_waypoint_struct()

        long_lat = int(waypoint_details.latitude * 1e6)
        long_long = int(waypoint_details.longitude * 1e6)

        waypoint_struct.pack_into(
            buff,
            buff_offset,
            long_lat,
            long_long,
            waypoint_details.accept_radius)

        return buff_offset + waypoint_struct.size

    def add_command_message_to_buffer(self, buffer, command_details : SailFaceCommand):
        command_struct = self.get_command_struct()

        num_waypoints = len(command_details.waypoints)

        command_struct.pack_into(
            buffer,
            0,
            command_details.prop_speed,
            command_details.bluetooth_active,
            command_details.update_frequency_minutes,
            command_details.waypoint_action,
            num_waypoints
        )

        offset = command_struct.calcsize()
        for waypoint in command_details.waypoints:
            offset = self._add_waypoint_to_buffer(buffer, offset, waypoint)

        return offset

    def build_command(self):

        buff = create_string_buffer(500)

    

    
    def get_command_struct(self):
        """
            typedef struct {

            // speed of prop between 0 and 255.
            short propSpeed;

            short bluetoothActive;

            // Time in seconds that status updates are sent through Iridium.
            long updateFrequencyMinutes;


            bool navigateToWaypoint;

            // Desired Waypoint
            // Actions
            //    Set to 0 for no change. (WAYPOINT_NO_CHANGE)
            //    Set to 1 delete existing waypoints and add this one (WAYPOINT_OVERWRITE)
            //    Set to 2 for append additional (WAYPOINT_APPEND)
            short waypointAction;

            short numWaypoints;
            Waypoint waypoints[];

        } IridiumCommandMessage;
    """
        fields = (
            ('propSpeed', 'h'),
            ('bluetoothActive', 'h'),
            ('updateFrequencyMinutes', 'l'), # should be d
            ('navigateToWaypoint', '?'), #

            ('waypointAction', 'h'), # this should be "d" doubles in Arduino are defined as 4 bytes not 8
            ('numWaypoints', 'h'),
        )

        # "<" represents little endian
        struct_format_str = '<' + ''.join([
            field[1]
            for field in fields
        ])

        return Struct(struct_format_str)

    def get_waypoint_struct(self):
        """
        typedef struct {
            long latitude;
            long longitude;

            // The radius in meters defining the circle centered on the waypoint
            // where the navigation subsystem will accept the waypoint as being reached.
            long allowedRadiusMeters;

        } Waypoint;
        """
        fields = (
            ('latitude', 'l'),
            ('longitude', 'l'), # should be d
            ('allowedRadiusMeters', 'l'), # should be d
        )

        # "<" represents little endian
        struct_format_str = '<' + ''.join([
            field[1]
            for field in fields
        ])

        return Struct(struct_format_str)