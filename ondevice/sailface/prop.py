from motor import DCMotor


PROP_MOTOR_ENABLE1 = 16
PROP_MOTOR_ENABLE2 = 17
PROP_MOTOR_PWM = 27

class MainPropControl:
    def __init__(self):
        self.motor = DCMotor(
            PROP_MOTOR_ENABLE1,
            PROP_MOTOR_ENABLE2,
            PROP_MOTOR_PWM)

    def forward(self, speed):
        """Set the propeller to move forward at the given speed (0-100)"""
        if speed <= 0:
            self.stop()

        self.motor.forward(speed)

    def stop(self):
        """Stop the propeller"""
        self.motor.stop()