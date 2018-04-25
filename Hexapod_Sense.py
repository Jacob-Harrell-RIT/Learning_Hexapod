from __future__ import print_function
import serial
#example received string
#a:X:-192,Y:160,Z:8256,m:X:349,Y:-158,Z:-527,s0:228,s1:351,s2:252,s3:354,s4:269,s5:262,
#real input string:
#-512,512,8176,371,-140,-529,302,306,410,392,291,325,
class hexSense:

    def __init__(self,port,speed):
        #port='/dev/ttyACM0', speed=115200
        self.port=serial.Serial(port,speed)
        if not self.port.is_open:
                self.port.open()
        self.port_speed=speed
        self.state_string=''
        
        self.state_array=[]
        self.vel=[0,0,0]
        self.accel=[0,0,0]
        self.rpy=[0,0,0]
        
        
        
        
    def __del__(self):
        if self.port.is_open:
            #close the port
            self.port.close()
            
      
           
    def rx_state(self):
        if self.port.is_open:
            while True:
                d1=[]
                if (self.port.inWaiting()): #get a whole line
                    #TODO deal with sending state data to learning algorithm
                    d1=self.port.readline(self.port.inWaiting())                    
                    self.state_string=d1
                    rx_state=self.state_string.split(",")
                    if len(rx_state)!=13:
                        return False
                    elif rx_state[12]!='\r\n':
                        return False
                    elif rx_state[0]=='':
                        return False
                    self.state_array=rx_state[0:12]
                    #print(self.state_array)
                    return True
        else:
            return False            




