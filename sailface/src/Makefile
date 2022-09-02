ARDUINO_CLI := ./bin/arduino-cli --config-file ./lib/arduino-cli.yaml
APP_NAME := sailface
ARDUINO_LIBS := \
	"Adafruit_INA219" \
	"Adafruit_BusIO" \
	"Iridium_SBD" \
	Servo \
	"SerialCommands-2.2.0" \
	TinyGPSPlus-1.0.2b \
	EpoxyEepromAvr \
	"MPU9250" \
	"Adafruit Unified Sensor" \
	"PID"

ARDUINO_LIB_DIR := ./lib
ARDUINO_LIB_DIRS := \
	./lib \
	./lib/libraries

include lib/EpoxyDuino/EpoxyDuino.mk


install-libraries:
	$(ARDUINO_CLI) lib install "Adafruit INA219"
	$(ARDUINO_CLI) lib install "Iridium SBD"
	$(ARDUINO_CLI) lib install Servo
	$(ARDUINO_CLI) lib install "SerialCommands"
	$(ARDUINO_CLI) lib install MPU9250
	$(ARDUINO_CLI) lib install "Adafruit BusIO"
	$(ARDUINO_CLI) lib install "Adafruit Unified Sensor"
	$(ARDUINO_CLI) lib install "PID"
	$(ARDUINO_CLI) lib install "AUnit"
