ARDUINO_CLI=~/sailface/bin/arduino-cli
ARDUINO_LIB_DIR=/home/pi/Arduino/libraries
BOARD_TYPE=arduino:avr:uno

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SAILFACE_DIR=$(ROOT_DIR)/bluetooth



compile:
	$(ARDUINO_CLI) compile -v --fqbn $(BOARD_TYPE) bluetooth
upload: compile
	$(ARDUINO_CLI) upload -v -p /dev/ttyUSB0 --fqbn $(BOARD_TYPE) bluetooth --input-dir $(SAILFACE_DIR)/build/arduino.avr.uno

clean:
	rm -rf $(SAILFACE_DIR)/build
