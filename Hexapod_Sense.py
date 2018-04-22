from __future__ import print_function
import serial
#example received string
#a:X:-192,Y:160,Z:8256,m:X:349,Y:-158,Z:-527,s0:228,s1:351,s2:252,s3:354,s4:269,s5:262,

class hexSense:

    def __init__(self,port,speed):
        #port='/dev/ttyACM0', speed=115200
        self.port=serial.Serial(port,speed)
        if not self.port.is_open:
                ser.open()
        self.port_speed=speed
        self.state=''
        
    def __del__(self):
        if self.port.is_open:
            #close the port
            self.port.close()
           
    def get_state(self):
        while True:
            if (self.port.inWaiting()>86): #get a whole line
                #TODO deal with sending state data to learning algorithm
                d1=self.port.read(self.port.inWaiting())
                self.state=d1
                break
                



