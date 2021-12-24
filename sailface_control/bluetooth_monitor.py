#!/usr/bin/python3
import struct
from serial import Serial
from serial.tools.list_ports import comports

def find_sailface_bluetooth_port():
    for port in comports():
        print(port, port.serial_number, port.name)
        if port.name == 'cu.HC-05-DevB':
            return port.device

    raise Exception('SERIAL PORT NOT FOUND!')


if __name__ == '__main__':
    port_path = find_sailface_bluetooth_port()

    print('FOUND SAIL FACE AT "%s".' % port_path)
    serial_port = Serial(port_path, 9600, timeout=5)


    """
    # Refer to sailface.h
    bool positionValid;
    double latitude;
    double longitude;
    uint32_t gpsFixAge;

    double course; // course in degrees
    double speed; // speed in knots

    // Power Management
    float batteryVoltage;       // voltage in Volts out of the sunny buddy chargin unit.
    float solarPanelCurrent;    // current in mA measured in series with the solar panels
    float batteryCurrentDraw;
    """
    fields = (
        ('position_valid', '?'),
        ('latitude', 'f'), # should be d
        ('longitude', 'f'), # should be d
        ('gps_fix_age', 'f'), #
        ('course', 'f'), # this should be "d" doubles in Arduino are defined as 4 bytes not 8
        ('speed', 'f'), # this should be "d"
        ('battery_voltage', 'f'),
        ('panel_current', 'f'),
        ('battery_current_draw', 'f')
    )

    # "<" represents little endian
    struct_format_str = '<' + ''.join([
        field[1]
        for field in fields
    ])



    sailface_struct = struct.Struct(struct_format_str)
    print('STRUCT FORMAT:', struct_format_str, 'LEN:', sailface_struct.size)

    while True:
        binary_packed_data = serial_port.read(sailface_struct.size)

        unpacked_data = sailface_struct.unpack(binary_packed_data)
        values = {
            fields[idx][0]: val
            for idx, val in enumerate(unpacked_data)
        }

        print('STRUCT FORMAT:', struct_format_str, 'LEN:', sailface_struct.size)
        for key, value in values.items():
            print('%s:\t%s' % (key, value))
        print()
        #print('FOUND MSG: ', values)
