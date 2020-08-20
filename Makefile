


ARDUINO_CLI=~/sailface/bin/arduino-cli
ARDUINO_LIB_DIR=/home/pi/Arduino/libraries
BOARD_TYPE=arduino:avr:uno
SAILFACE_DIR=~/sailface/sailface


install-libraries:
	$(ARDUINO_CLI) lib install "Adafruit INA219"
	$(ARDUINO_CLI) lib install Servo
	rm -rf /home/pi/Arduino/libraries/TinyGPS++/
	ln -s $(SAILFACE_DIR)/lib/TinyGPSPlus-1.0.2b $(ARDUINO_LIB_DIR)/TinyGPS++

compile:
	$(ARDUINO_CLI) compile -v --fqbn $(BOARD_TYPE) sailface
upload:
	$(ARDUINO_CLI) upload -v -p /dev/ttyUSB0 --fqbn $(BOARD_TYPE) sailface --input-dir $(SAILFACE_DIR)/build/arduino.avr.uno

