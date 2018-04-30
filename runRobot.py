import SSC32
from time import sleep
import sys
import os
import math as mathypoo
QMatrix=[];
tmp=0

ch=[1,0,17,16,21,5,20,4]
hexapod=SSC32.SSC32_Multi_Servo('/dev/ttyUSB0',115200,ch)

if __name__ == '__main__':
    try:
        with open('values.txt','r') as fname:
            for line in fname:
                QMatrix.append([]);
                QMatrix[tmp]=[float(n) for n in line.strip(',\n').split(',')]
                tmp += 1
                    
        for i in range(0, len(QMatrix)):
	    # print(QMatrix[i])
            for j in range(0, len(QMatrix[i])):
		#print(QMatrix[i][j])
		#16
		#04
		if (j==0 or j==6 ):
			QMatrix[i][j] = -QMatrix[i][j]
		# 0345
		#1672
		elif (j==1 or j==3 or j==4 or j==5):
			QMatrix[i][j] *= 5
                QMatrix[i][j] *= 1000
                QMatrix[i][j] += 2000
                QMatrix[i][j] /= 4
                QMatrix[i][j] += 1000
                QMatrix[i][j] = mathypoo.floor(QMatrix[i][j])
                QMatrix[i][j] = int(QMatrix[i][j])
            hexapod.send_joint_state(QMatrix[i])
            sleep(0.25)
	    print(i)
    except KeyboardInterrupt:
        try:
            hexapod.__del__()
            sys.exit(0)
        except SystemExit:
            hexapod.__del__()
            os._exit(0)
    
