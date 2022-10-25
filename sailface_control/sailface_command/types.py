from typing import List
from dataclasses import dataclass

# these constants are defined in Iridium.h
WAYPOINT_NO_CHANGE = 0
WAYPOINT_OVERWRITE = 1
WAYPOINT_APPEND = 2

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