"""Control a DC Motor with an L298N Motor Driver"""

from machine import Pin, PWM


class DCMotor:    
    """Control a DC Motor with an L298N Motor Driver"""

    # TODO: Adjust the minimum duty cycle based on the motor
    # to get a good range of different speeds. 
    def __init__(self, pin1, pin2, enable_pin, min_duty=550, max_duty=1023):
        self.pin1=Pin(pin1, Pin.OUT)
        self.pin2=Pin(pin2, Pin.OUT)

        frequency = 15000
        self.enable_pin=PWM(Pin(enable_pin), frequency)
        self.min_duty = min_duty
        self.max_duty = max_duty

    """
    WARNING WARNING WARNING. I put a snubber diode directly on the motor to prevent EMF.
    DO NOT PUT THE MOTOR IN REVERSE! THIS WILL RESULT IN A SHORT!
    """

    def forward(self,speed):
        self.speed = speed
        self.enable_pin.duty(self.duty_cycle(self.speed))
        self.pin1.value(1)
        self.pin2.value(0)

    """
    def backwards(self, speed):
        self.speed = speed
        self.enable_pin.duty(self.duty_cycle(self.speed))
        self.pin1.value(0)
        self.pin2.value(1)
    """

    def stop(self):
        self.enable_pin.duty(0)
        self.pin1.value(0)
        self.pin2.value(0)
        
    def duty_cycle(self, speed):
        if speed <= 0 or speed > 100:
            duty_cycle = 0
        else:
            duty_cycle = int(self.min_duty + (self.max_duty - self.min_duty)*((speed-1)/(100-1)))

        return duty_cycle

# Path: development/sailface-esp32/ondevice/vendor/motor.py

#from dcmotor import DCMotor       
#from time import sleep     

#DCMotor1 = DCMotor(16, 17, 27)
#DCMotor.forawrd(50)
