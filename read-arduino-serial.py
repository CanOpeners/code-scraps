#!/usr/bin/env python3

from sys import argv
import serial

print('\x1b[33m    Ctrl-C to stop reading\x1b[m')

try:
    sernum = argv[1]
except:
    sernum = '0'

with serial.Serial('/dev/ttyACM' + sernum) as ser:
    while True:
        byte = ser.read()
        print(byte.decode('ASCII'), end='')
