#include <MatrixMath.h>
#include <TimerOne.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303.h>
#define MatDim 4 //matrices all 4x4

volatile int accelRDY=0;//interrupt flag
Adafruit_LSM303 lsm;//ref to LSM
const int analogInPin = A0;//1st pin to read on analog reads
//flags for first iterations
int first_acc_done=0;
int first_acc_count=0;
//end flags
//sensor stuff
volatile int sensorValue = 0; 
//time since last loop ended
unsigned long int now=0;

/*---STARTING POSE---*/
double Start_ACC[MatDim][MatDim];//initial orientation of accel
double Start_Inv_ACC[MatDim][MatDim];//inverse of the starting accelerometer
double Start_MAG[MatDim][MatDim];//initial orientation of mag
double Start_Inv_MAG[MatDim][MatDim];//inverse of starting mag
/*---END STARTING POSE---*/

/*CURRENT MEASURED STATE*/
double Acc_Matrix[MatDim][MatDim];//accelerometer as pxpypz
double Mag_Matrix[MatDim][MatDim];//Hold the magnetometer oreintation as rpy
double Mag_Inv[MatDim][MatDim];//magnetometer inverse
/*-END CURRENT MEASURED STATE-*/

/*COMPENSATED VALUES*/
double Comp_Mag[MatDim][MatDim];//hold compensated mag angles
double Comp_Accel[MatDim][MatDim];//hold compensated accelerometer data

/*Temporary Storage*/
double TMP_Matrix[MatDim][MatDim];//temporary matrix for swaps
double Mag_Inv_Tmp[MatDim][MatDim];
double Accel_Inv_Tmp[MatDim][MatDim];
void setup(void)
{
  Serial.begin(115200);
  pinMode(2,INPUT);//pin 2 is interrupt trigger
  pinMode(13,OUTPUT);
  digitalWrite(13,0);//LED OFF
  /*set all matrices to 0*/
  Matrix.Zeros((double*)Start_ACC,MatDim,MatDim);
  Matrix.Zeros((double*)Start_Inv_ACC,MatDim,MatDim);
  Matrix.Zeros((double*)Start_MAG,MatDim,MatDim);
  Matrix.Zeros((double*)Start_Inv_MAG,MatDim,MatDim);
  Matrix.Zeros((double*)Acc_Matrix,MatDim,MatDim);
  Matrix.Zeros((double*)Mag_Matrix,MatDim,MatDim);
  Matrix.Zeros((double*)Comp_Mag,MatDim,MatDim);
  Matrix.Zeros((double*)Comp_Accel,MatDim,MatDim);
  Matrix.Zeros((double*)TMP_Matrix,MatDim,MatDim);
  Matrix.Zeros((double*)Mag_Inv_Tmp,MatDim,MatDim);
  Matrix.Zeros((double*)Accel_Inv_Tmp,MatDim,MatDim);
  /*---setup_timer---*/
  Timer1.initialize(10000000);
  Timer1.stop();
  /*end timer setup*/
  if(!lsm.begin())//start the LSM303
  {//if the LSM303 doesn't start
    /* There was a problem detecting the ADXL345 ... check your connections */
    while(1){
      //slow blink and busy loop
      digitalWrite(13,1);
      delay(1000);
      digitalWrite(13,0);
      delay(1000);
    }
  }//otherwise go.
  attachInterrupt(0,LSM_RDY,FALLING);
  Timer1.restart();
  now=Timer1.read();
}

long int init_accel[3]={0,0,0};//hold initial orientation
long int init_mag[3]={0,0,0};

void loop(){
  if(accelRDY==1){
    now=Timer1.read();//how long has it been?
    lsm.read();//read IMU every loop
    //hold read data
    long int accelxyz[3]={(long int)lsm.accelData.x,(long int)lsm.accelData.y,(long int)lsm.accelData.z};
    long int magxyz[3]={(long int)lsm.magData.x,(long int)lsm.magData.y,(long int)lsm.magData.z};
    //initial setup
    if(first_acc_done==0){
      if(first_acc_count<10){
       for(int k=0;k<3;k++){
         init_accel[k]+=accelxyz[k];
         init_mag[k]+=magxyz[k];
       }      
        first_acc_count++;
      }
      else{
         for(int k=0;k<3;k++){
           init_accel[k]/=10;//average of init accelerometer reads 
           init_mag[k]/=10;//average of init mag data
         }
         
         /*---get initial orientation and compensation ---*/
         double tmp_mag_orient[3]={0,0,0};//hold the magnetometer orientation for now
         MeasToRPY((double*)tmp_mag_orient,(double)init_mag[0],(double)init_mag[1],(double)init_mag[2],0.04);
         Matrix.RPYtoMatrix((double*)Start_MAG,MatDim,MatDim,tmp_mag_orient[0],tmp_mag_orient[1],tmp_mag_orient[2]);
         Matrix.Copy((double*)Start_MAG,MatDim,MatDim,(double*)Start_Inv_MAG);
         Matrix.Invert((double*)Start_Inv_MAG,MatDim);
         /*end get mag compensation values*/
         /*--get initial accelerometer read and compensation--*/
         Matrix.XYZtoMatrix((double*)Start_ACC,MatDim,MatDim,(double)init_accel[0],(double)init_accel[1],(double)init_accel[2]);
         Matrix.Copy((double*)Start_ACC,MatDim,MatDim,(double*)Start_Inv_ACC);
         Matrix.Invert((double*)Start_Inv_ACC,MatDim);
         /*---end Accelerometer compensation---*/
         first_acc_done=1;//now go to normal operation   
      }
      
    }
    //normal operation
    else{
      //convert magnetometer to RPY
      double mag_orient[3]={0,0,0};
      MeasToRPY((double*)mag_orient,(double)magxyz[0],(double)magxyz[1],(double)magxyz[2],0.04);
      //store current mag state
      Matrix.RPYtoMatrix((double*)Mag_Matrix,MatDim,MatDim,mag_orient[0],mag_orient[1],mag_orient[2]);
      //compensate magnetometer reading
      Matrix.Multiply((double*)Mag_Matrix,(double*)Start_Inv_MAG,MatDim,MatDim,MatDim,(double*) Comp_Mag);
      //get matrix of magnetometer reading
      //invert magnetometer reading
      Matrix.Copy((double*)Comp_Mag,MatDim,MatDim,(double*)Mag_Inv_Tmp);
      Matrix.Invert((double*)Mag_Inv_Tmp,MatDim);
      
      Matrix.XYZtoMatrix((double*)Acc_Matrix,MatDim,MatDim,(double)accelxyz[0],(double)accelxyz[1],(double)accelxyz[2]);
      //compensate accelerometer
      Matrix.Multiply((double*)Acc_Matrix,(double*)Start_Inv_ACC,MatDim,MatDim,MatDim,(double*)Comp_Accel);
      Matrix.Copy((double*)Comp_Accel,MatDim,MatDim,(double*)TMP_Matrix);
      //compensate accel with magnetometer read
      Matrix.Multiply((double*)TMP_Matrix,(double*)Mag_Inv_Tmp,MatDim,MatDim,MatDim,(double*)Comp_Accel);
      //Matrix.Print((double*)Comp_Mag,MatDim,MatDim,"magcomp");
      Serial.print(Comp_Accel[0][3]);
      Serial.print(',');
        for(int n=0;n<=3;n++){
          sensorValue = analogRead(analogInPin+n);
          Serial.print(sensorValue);   
          Serial.print(",");
         }
         Serial.print(now);
         Serial.println(',');
      }
    //indicate loop done, clear flag
    digitalWrite(13,0);
    accelRDY=0;
    Timer1.restart();
  }
  
}


void LSM_RDY(void){//interrupt routine
  accelRDY=1;
  digitalWrite(13,1);
 }
 
 //convert xyz to rpy based on ST's reference Doc
 void MeasToRPY(double* RPY_angles, double x_meas, double y_meas, double z_meas,double alpha){
 //takes a pointer to a 3 wide array
 RPY_angles[0]=atan2((double)y_meas,((double)z_meas+(double)x_meas*alpha));//roll
 double Tmpz2=(double)y_meas*sin(RPY_angles[0])+((double)z_meas+(double)x_meas*alpha)*cos(RPY_angles[0]);
 RPY_angles[1]=atan(-((double)x_meas)/Tmpz2);
 double Tmpy2=((double)z_meas+(double)x_meas*alpha)*sin(RPY_angles[0])-(double)y_meas*cos(RPY_angles[0]);
 double Tmpx2=(double)x_meas*cos(RPY_angles[1])-((double)z_meas+(double)x_meas*alpha)*sin(RPY_angles[1]);
 RPY_angles[2]=atan2(Tmpy2,Tmpx2);
 }
 
 
