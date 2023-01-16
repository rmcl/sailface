from machine import Pin, PWM


RUDDER_PWM_PIN = 33
RUDDER_SERVO_FREQ = 50
RUDDER_SERVO_DUTY_MIN = 40
RUDDER_SERVO_DUTY_MAX = 115


class Rudder:
    """Control the rudder servo."""

    def __init__(self, default_position=0):
        """Initialize the rudder servo."""
        self.rudder_pwm = PWM(
            Pin(RUDDER_PWM_PIN),
            freq=RUDDER_SERVO_FREQ)

        self.set_position(default_position)

    def set_position(self, pos):
        """ Set the rudder position between -10 and 10. """
        if pos < -10:
            pos = -10
        elif pos > 10:
            pos = 10
            
        duty = RUDDER_SERVO_DUTY_MIN + (pos + 10) * (RUDDER_SERVO_DUTY_MAX - RUDDER_SERVO_DUTY_MIN) / 20
        self.rudder_pwm.duty(int(duty))
