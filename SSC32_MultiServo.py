#example to control an ssc32
#TODO:rewrite as class

from __future__ import print_function
import sys
import os
import serial


def send_joint_state(ser, joint_state=[], joint_channels=[]):
    if ser.is_open:
        ssc32_MSG=''
        for k in range(0,len(joint_channels)):
            ssc32_MSG=ssc32_MSG+'#'
            ssc32_MSG=ssc32_MSG+str(joint_channels[k])
            ssc32_MSG=ssc32_MSG+'P'
            ssc32_MSG=ssc32_MSG+str(joint_state[k])
            
        ssc32_MSG=ssc32_MSG+'\r\n'
        print(ssc32_MSG,end='')
        ser.write(ssc32_MSG) #send your joint states and channels to the SSC32 all at once
    else:
        print('serial port not open',end='\n')
        

if __name__ == '__main__':
    try:                 
        ser=serial.Serial('/dev/ttyUSB0', 115200)
        print(ser.is_open)
        if not ser.is_open:
            ser.open()'
        #arbitrary joint states, will be dynamic in the future
        state=[1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500]
        ch=[0,1,2,3,4,5,21,20,19,18,17,16]
        send_joint_state(ser,state,ch)

    except KeyboardInterrupt:
        try:          
            sys.exit(0)
        except SystemExit:
            os._exit(0)

