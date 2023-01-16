import logging
from logging import (
    Formatter,
    StreamHandler,
    _loggers as defined_loggers
)
from logging.handlers import RotatingFileHandler


formatter = Formatter('%(asctime)s::%(name)s::%(message)s')

console_handler = StreamHandler()
console_handler.setFormatter(formatter)

sd_file_handler = None

def get_logger(name, level=logging.INFO):
    logger = logging.getLogger(name)
    logger.addHandler(console_handler)
    
    if sd_file_handler:
        print('add sd handler')
        logger.addHandler(sd_file_handler)

    logger.setLevel(level)
    return logger

def setup_sd_log_handler():
    print('called setup sd log')
 
    global sd_file_handler
    sd_file_handler = RotatingFileHandler('/sd/sailface-log.txt')
    sd_file_handler.setFormatter(formatter)

    for existing_loggers in defined_loggers.values():
        existing_loggers.addHandler(sd_file_handler)
