import logging
from logging import StreamHandler, Formatter
from sailface.logs import get_logger
from unittest import TestCase
from unittest.mock import Mock
#from sailface.log_handlers import ConsoleHandler


class ConsoleHandlerTest(TestCase):

    def test_setup_logger_with_console(self):
        logger = get_logger('sailboard-main')
        logger.error({'hello': "WORLD"})

    