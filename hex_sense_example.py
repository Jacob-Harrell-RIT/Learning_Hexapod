from __future__ import print_function
import Hexapod_Sense
import sys
import os

if __name__ == '__main__':
    try:
        mega=Hexapod_Sense.hexSense('/dev/ttyACM0',115200)
        #mega.port.close()
        #mega=Hexapod_Sense.hexSense('/dev/ttyACM0',115200)
        while True:
            
            if mega.rx_state():
                print(mega.state_array)
            #print(mega.state_string, end='')
            else:
                print('-')
            
    except KeyboardInterrupt:
        try:
            mega.__del__()#close the port
            sys.exit(0)
        except SystemExit:
            os._exit(0)


