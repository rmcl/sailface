"""Main entrypoint for ESP32 running micropython."""
from sailface.sailboard import SailboardMain

sailboard = SailboardMain()
sailboard.setup()
sailboard.loop()
