import logging
import json
import urequests
from logging import (
    Formatter,
    StreamHandler,
    Handler,
    _loggers as defined_loggers
)
from logging.handlers import RotatingFileHandler
from .utils import get_config

formatter = Formatter('%(asctime)s::%(name)s::%(message)s')

console_handler = StreamHandler()
console_handler.setFormatter(formatter)

flash_file_handler = RotatingFileHandler('./flash-sailface-log.txt')
flash_file_handler.setFormatter(formatter)

api_handler = None

sd_file_handler = None

def get_logger(name, level=logging.INFO):
    logger = logging.getLogger(name)
    logger.addHandler(console_handler)
    
    if sd_file_handler:
        print('add sd handler')
        logger.addHandler(sd_file_handler)
    else:
        logger.addHandler(flash_file_handler)

    if api_handler:
        logger.addHandler(api_handler)

    logger.setLevel(level)
    return logger

def setup_sd_log_handler():
    print('called setup sd log')
 
    global sd_file_handler
    sd_file_handler = RotatingFileHandler('/sd/sailface-log.txt')
    sd_file_handler.setFormatter(formatter)

    for existing_loggers in defined_loggers.values():
        existing_loggers.addHandler(sd_file_handler)


class TelemetryAPILog(Handler):
    """A log handler that writes messages to an API endpoint."""

    def __init__(self):
        super().__init__()

        config = get_config()
        self.telemetry_config = config['telemetry']

        self.network_error_fp = open('/sd/network-error-log.txt', 'a+')

    def log_network_error(self, message):
        """Write to file."""
        self.network_error_fp.write(message)


    def emit(self, record):
        """Write to file."""
        msg = self.formatter.format(record)

        record_details = {
            'formatted_message': msg,
            'message': record.message,
            'level': record.levelname,
            'log_name': record.name,
            'timestamp': record.created
        }

        try:
            result = urequests.post(
                self.telemetry_config['url'],
                data=json.dumps(record_details),
                headers={
                    'Content-Type': 'application/json',
                    'x-api-key': self.telemetry_config['api_key']
                })

            result.close()
        except OSError as error:
            print('Failed to post to telemetry api: ' + str(error))
            self.log_network_error("Failed to write to telemetry api: " + str(error))


def setup_telemetry_api_log_handler():
    print('called setup telemetry api logger')

    global api_handler
    api_handler = TelemetryAPILog()
    api_handler.setFormatter(formatter)

    for existing_loggers in defined_loggers.values():
        existing_loggers.addHandler(api_handler)