import os
from machine import SDCard
from .constants import SDLOG_DIR

class DataLogger:
    def __init__(self):
        self.log_fps = {}

    def log(self, log_name, data):
        if log_name not in self.log_fps:
            self.log_fps[log_name] = open(log_name, 'a+')

        self.log_fps[log_name].write(data)