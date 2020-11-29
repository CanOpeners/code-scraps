#!/usr/bin/env python3

import serial

print('\x1b[33m    Ctrl-C to stop reading\x1b[m')

with serial.Serial('/dev/ttyACM0') as ser:
    while True:
        byte = ser.read()
        print(byte.decode('ASCII'), end='')
