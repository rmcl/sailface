


ARDUINO_CLI_EXE=~/sailface/bin/arduino-cli
BOARD_TYPE=arduino:avr:uno
SAILFACE_DIR=~/sailface/sailface


compile:
	$(ARDUINO_CLI_EXE) compile -v --fqbn $(BOARD_TYPE) sailface
upload:
	$(ARDUINO_CLI_EXE) upload -v -p /dev/ttyUSB0 --fqbn $(BOARD_TYPE) sailface --input-dir $(SAILFACE_DIR)/build/arduino.avr.uno

