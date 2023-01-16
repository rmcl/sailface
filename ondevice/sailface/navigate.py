from PID import PID

class Navigate:

    def __init__(self):
        """Initialize the navigation."""
        self._heading = None
        self.pid = None


    def set_heading(self, heading):
        """Set the heading of the craft."""
        self._heading = heading

        self.pid = PID(
            Kp=1.0,
            Ki=0.0,
            Kd=0.0,
            setpoint=self._heading,

            # Sample time is every 5 seconds
            sample_time=5,
            scale='s',
            output_limits=[None, None])

    def update_and_adjust_rudder(self, rudder_manager):
        """Update the heading of the craft."""
        bearing = self._position.get_bearing()

        output = self.pid.update(bearing)

        print(
            f'desired heading: {self._heading}, bearing: {bearing}, output: {output}')