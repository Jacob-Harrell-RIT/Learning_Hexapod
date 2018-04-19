from __future__ import print_function
import threading
from time import sleep
import sys
import os
import serial
#TODO: add serial for lynxmotion ssc32

def mega_get_state(ser,con_1):
    while not con_1:
        con1=True
        while True:
             if (ser.inWaiting()>36): #get a whole line
                #TODO deal with sending state data to learning algorithm
                d1=ser.read(ser.inWaiting())
                print(d1,end='')
                
if __name__ == '__main__':
    try:                 
        ser=serial.Serial('/dev/ttyACM1', 9600)
        conn=False
        mega_get_state(ser,conn)

    except KeyboardInterrupt:
        try:          
            sys.exit(0)
        except SystemExit:
            os._exit(0)
