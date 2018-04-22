from __future__ import print_function
import Hexapod_Sense
import sys
import os

if __name__ == '__main__':
    try:
        mega=Hexapod_Sense.hexSense('/dev/ttyACM0',115200)
        while True:
            mega.get_state()
            print(mega.state, end='')
    except KeyboardInterrupt:
        try:          
            sys.exit(0)
        except SystemExit:
            os._exit(0)
