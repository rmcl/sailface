#!/usr/bin/python3

import sys
from datetime import datetime
from serial import Serial
from serial.tools.list_ports import comports

def find_sailface_port():
    allowed_ports = [
        #'cu.usbmodem14101',
        'cu.HC-05-DevB',
    ]
    for port in comports():
        print(port, port.serial_number, port.name)
        if port.name in allowed_ports:
            return port.device

    raise Exception('SERIAL PORT NOT FOUND!')


if __name__ == '__main__':
    port_path = find_sailface_port()

    print('FOUND SAIL FACE AT "%s".' % port_path)
    #serial_port = Serial(port_path, 115200, timeout=5)

    # baud_rate = 115200
    baud_rate = 9600

    serial_port = Serial(port_path, baud_rate, timeout=30)

    out_fp = open('output_binary.bin', 'ab+')
    while True:
        message = serial_port.readline()

        out_fp.write(message)
        out_fp.flush()

        try:

            message = message.decode('ascii').replace('\n','')
            print(datetime.now(), '\t', message)
        except:
            print('malformed message')

        sys.stdout.flush()
