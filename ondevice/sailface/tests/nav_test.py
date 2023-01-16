"""
from navigate import Navigate
from position import Position
from rudder import Rudder


position_manager = Position()
navigation = Navigate()
rudder = Rudder()


def main():
    navigation.set_heading(90)

    for _ in range(100):
        position_manager.update()
    
        navigation.update_and_adjust_rudder(rudder)
"""