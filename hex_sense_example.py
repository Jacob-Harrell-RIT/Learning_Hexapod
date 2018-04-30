from __future__ import print_function
import Hexapod_Sense
import sys
import os
from time import sleep
mega=Hexapod_Sense.hexSense('/dev/ttyACM0',115200)

if __name__ == '__main__':
    try:
        
        #mega.port.close()
        #mega=Hexapod_Sense.hexSense('/dev/ttyACM0',115200)
        while True:
            
            if mega.rx_state():
                print(mega.state_array)
            sleep(0.015)

    except KeyboardInterrupt:
        try:
            mega.__del__()#close the port
            sys.exit(0)
        except SystemExit:
            os._exit(0)
    finally:
            print('err');
            mega.__del__()
