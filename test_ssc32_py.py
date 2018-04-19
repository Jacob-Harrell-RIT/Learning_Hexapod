import SSC32
from time import sleep
import sys
import os

if __name__ == '__main__':

    try:
        ch=[0,1,2,3,4,5,21,20,19,18,17,16]
        hexapod=SSC32.SSC32_Multi_Servo('/dev/ttyUSB0',115200,ch)
        state=[1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500]
        hexapod.send_joint_state(state)
        n=0;
        while n<10:
            print(10-n)
            sleep(0.25)
            n=n+1
            
        hexapod.__del__() #go back to neutral state
        
    except KeyboardInterrupt:
        try:
            hexapod.__del__()
            sys.exit(0)
        except SystemExit:
            hexapod.__del__()
            os._exit(0)
