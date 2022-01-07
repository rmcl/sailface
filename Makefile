ARDUINO_CLI=/Users/rmcl/Documents/Arduino/sailface/bin/arduino-cli
ARDUINO_LIB_DIR=/Users/rmcl/Documents/Arduino/libraries
BOARD_TYPE=arduino:avr:mega

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SAILFACE_DIR=$(ROOT_DIR)/sailface
SAILFACE_CONTROL_DIR=$(ROOT_DIR)/sailface_control
USB_DEVICE=/dev/cu.usbserial-1410

install-libraries:
	$(ARDUINO_CLI) lib install "Adafruit INA219"
	$(ARDUINO_CLI) lib install "Iridium SBD"
	$(ARDUINO_CLI) lib install Servo
	curl -L "https://github.com/mikalhart/TinyGPSPlus/archive/v1.0.2b.zip" --output "$(ARDUINO_LIB_DIR)/v1.0.2b.zip"
	unzip "$(ARDUINO_LIB_DIR)/v1.0.2b.zip" -d $(ARDUINO_LIB_DIR)
	rm "$(ARDUINO_LIB_DIR)/v1.0.2b.zip"
	curl https://downloads.arduino.cc/libraries/github.com/sparkfun/Serial_Controlled_Motor_Driver-1.0.4.zip --output "$(ARDUINO_LIB_DIR)/Serial_Controlled_Motor_Driver-1.0.4.zip"
	unzip "$(ARDUINO_LIB_DIR)/Serial_Controlled_Motor_Driver-1.0.4.zip" -d $(ARDUINO_LIB_DIR)
	rm "$(ARDUINO_LIB_DIR)/Serial_Controlled_Motor_Driver-1.0.4.zip"

install-test-libraries:
	$(ARDUINO_CLI) lib install "AUnit"
	git clone "https://github.com/bxparks/EpoxyDuino.git" "$(ARDUINO_LIB_DIR)/EpoxyDuino"

install-control:
	sudo rm /etc/systemd/system/sailface.service
	sudo cp $(SAILFACE_CONTROL_DIR)/sailface.systemd.conf /etc/systemd/system/sailface.service
	sudo systemctl daemon-reload
	sudo systemctl enable sailface.service
	sudo systemctl status sailface.service

compile:
	$(ARDUINO_CLI) compile -v --fqbn $(BOARD_TYPE) sailface

upload: compile
	$(ARDUINO_CLI) upload -v -p $(USB_DEVICE) --fqbn $(BOARD_TYPE) sailface --input-dir $(SAILFACE_DIR)

compile-tests:
	APP_NAME := test_navigation
	ARDUINOLIBS := AUnit,$(ARDUINO_LIB_DIR)
	include /Users/rmcl/Documents/Arduino/libraries/EpoxyDuino/EpoxyDuino.mk


clean:
	rm -rf $(SAILFACE_DIR)/build
