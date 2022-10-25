import json
import os
from sailface_command.types import Waypoint

GROUNDCONTROL_MISSION_PATH = '/Users/rmcl/Documents/QGroundControl/Missions'

class GroundControlImporter:
    """Import the QGroundControl plan file and create waypoints."""
    
    def __init__(self, mission_dir_path = None):
        if mission_dir_path:
            self.mission_dir_path = mission_dir_path
        else:
            self.mission_dir_path = GROUNDCONTROL_MISSION_PATH

    def list_plans(self):
        return os.listdir(self.mission_dir_path)

    def load_plan(self, plan_name):
        with open(f'{self.mission_dir_path}/{plan_name}.plan', 'r') as fp:
            mission = json.load(fp)


        steps = []        
        for step in mission['mission']['items']:
            _,accept_radius_meters,_,_,latitude,longitude,_ = step['params']
            
            steps.append(Waypoint(
                latitude=latitude,
                longitude=longitude, 
                accept_radius=accept_radius_meters)
            )

        return steps