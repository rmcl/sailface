#!/usr/bin/python3

from serial import Serial
from serial.tools.list_ports import comports

def find_sailface_port():
    for port in comports():
        print(port, port.serial_number, port.name)
        if port.name == 'ttyUSB0':
            return port.device

    raise Exception('SERIAL PORT NOT FOUND!')
        

if __name__ == '__main__':
    port_path = find_sailface_port()

    print('FOUND SAIL FACE AT "%s".' % port_path)
    serial_port = Serial(port_path, 115200, timeout=5)

    while True:
        message = serial_port.readline()
        print(message.decode('ascii'))
