import math
from binascii import hexlify
from typing import List
from struct import Struct
from ctypes import create_string_buffer, Array
from .types import Waypoint, SailFaceCommand

class SatCommandBuilder:

    def get_command(self, command_details : SailFaceCommand):
        """Return the hex representation of encoded command message."""
        buff = create_string_buffer(500)
        offset = self.add_command_message_to_buffer(
            buff,
            command_details)
        
        return hexlify(buff[0:offset])

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
        accept_radius = math.ceil(waypoint_details.accept_radius)

        waypoint_struct.pack_into(
            buff,
            buff_offset,
            long_lat,
            long_long,
            accept_radius)

        return buff_offset + waypoint_struct.size

    def add_command_message_to_buffer(self, buffer : Array, command_details : SailFaceCommand) -> int:
        """Add a command message to a buffer.
        
        Returns the offset in the buffer to the end of the message.
        """
        command_struct = self.get_command_struct()

        num_waypoints = len(command_details.waypoints)

        command_struct.pack_into(
            buffer,
            0,
            command_details.prop_speed,
            command_details.bluetooth_active,
            command_details.update_frequency_minutes,
            command_details.navigate_to_waypoint,
            command_details.waypoint_action,
            num_waypoints
        )

        offset = command_struct.size
        for waypoint in command_details.waypoints:
            offset = self.add_waypoint_to_buffer(buffer, offset, waypoint)

        return offset

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