


ARDUINO_CLI=~/sailface/bin/arduino-cli
ARDUINO_LIB_DIR=/home/pi/Arduino/libraries
BOARD_TYPE=arduino:avr:uno

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SAILFACE_DIR=$(ROOT_DIR)/sailface
SAILFACE_CONTROL_DIR=$(ROOT_DIR)/sailface_control


install-libraries:
	$(ARDUINO_CLI) lib install "Adafruit INA219"
	$(ARDUINO_CLI) lib install Servo
	wget https://github.com/mikalhart/TinyGPSPlus/archive/v1.0.2b.zip -O "$(ARDUINO_LIB_DIR)/v1.0.2b.zip"
	unzip "$(ARDUINO_LIB_DIR)/v1.0.2b.zip" -d $(ARDUINO_LIB_DIR)
	rm "$(ARDUINO_LIB_DIR)/v1.0.2b.zip"

install-control:
	sudo rm /etc/systemd/system/sailface.service
	sudo cp $(SAILFACE_CONTROL_DIR)/sailface.systemd.conf /etc/systemd/system/sailface.service
	sudo systemctl daemon-reload
	sudo systemctl enable sailface.service
	sudo systemctl status sailface.service

compile:
	$(ARDUINO_CLI) compile -v --fqbn $(BOARD_TYPE) sailface
upload:
	$(ARDUINO_CLI) upload -v -p /dev/ttyUSB0 --fqbn $(BOARD_TYPE) sailface --input-dir $(SAILFACE_DIR)/build/arduino.avr.uno

clean:
	rm -rf $(SAILFACE_DIR)/build
