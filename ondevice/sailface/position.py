from vendor.QMC5883 import QMC5883L

class Position:
    """Determine the position and heading of the craft"""

    def __init__(self):
       self.compass = QMC5883L()
       self._bearing = None

    def update(self):
        self._bearing = self.compass.get_bearing_raw()

    @property
    def bearing(self):
        return self._bearing