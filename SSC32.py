#class to run the SSC32 with pyserial
#TODO -range handling, angle diffs, joint angles to time
from __future__ import print_function
import serial

class SSC32_Multi_Servo:

    def __init__(self,port,speed,channels=[]):
        if False:#len(channels)!=len(ranges):#TODO add range handling
            print('channels and ranges must be the same lengths')
            return 0
        else:
            self.channels=channels
            #self.ranges=ranges TODO add ranges 
            #ser=serial.Serial('/dev/ttyUSB0', 115200) old command example
            self.port=serial.Serial(port, speed)
            if not self.port.is_open:
                ser.open()
                
    def __del__(self):
        #close port on garbage collection
        if self.port.is_open:
            #turn the servos off
            idle_state=[0]*len(self.channels)
            self.send_joint_state(idle_state)
            #close the port
            self.port.close() 
        
        
    def send_joint_state(self,joint_state=[]):
        #concatenate joint states and send to SSC32 over serial
        #TODO handle there not being a port open
        # link to ref doc
        #  http://www.lynxmotion.com/images/data/lynxmotion_ssc-32u_usb_user_guide.pdf
        if self.port.is_open:
            ssc32_MSG=''
            for k in range(0,len(self.channels)):
                ssc32_MSG=ssc32_MSG+'#'
                ssc32_MSG=ssc32_MSG+str(self.channels[k])
                ssc32_MSG=ssc32_MSG+'P'
                ssc32_MSG=ssc32_MSG+str(joint_state[k])
            ssc32_MSG=ssc32_MSG+'\r\n'
            print(ssc32_MSG,end='')
            self.port.write(ssc32_MSG)
        else:
            print('serial port not open',end='\n')
        


