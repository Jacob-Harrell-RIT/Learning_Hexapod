from __future__ import print_function
import threading
from time import sleep
import serial
#TODO: add serial for lynxmotion ssc32

ser=serial.Serial('/dev/ttyACM1', 115200)
con_1=False

def mega_get_state(ser):
    while not con_1:
        con1=True
        while True:
             if (ser.inWaiting()>6): #get a whole line
             #TODO expand to arbitrary num of reads, deal with sending state data to learning algorithm
                             d1=ser.read(ser.inWaiting())
                print(d1,end='')
           
           

thread=threading.Thread(target=mega_get_state, args=(ser,))
thread.start()
