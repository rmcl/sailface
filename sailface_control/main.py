from serial import Serial
from serial.tools.list_ports import comports

def find_sailface_port():
    for port in comports():
        if port.serial_number == 'DN02Z3SW':
            return port.device


if __name__ == '__main__':
    port_path = find_sailface_port()

    print('FOUND SAIL FACE AT "%s".' % port_path)
    serial_port = Serial(port_path, 115200, timeout=5)

    while True:
        message = serial_port.readline()
        print(message.decode('ascii'))
