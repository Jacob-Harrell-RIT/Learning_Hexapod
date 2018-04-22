#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303.h>

volatile int accelRDY=0;
Adafruit_LSM303 lsm;

void setup(void)
{
  Serial.begin(115200);
  pinMode(2,INPUT);
  pinMode(13,OUTPUT);
  digitalWrite(13,0);
  /* Initialise the sensors */
  if(!lsm.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("no LSM303 detected");
    while(1){
      digitalWrite(13,1);
      delay(100);
      digitalWrite(13,0);
      delay(100);
    }
  }
  attachInterrupt(0,LSM_RDY,FALLING);
}

void loop(void)
{
  /* Get a new sensor event */
  if(accelRDY==1){
    lsm.read();

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("Accel X: "); Serial.print((int)lsm.accelData.x); Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)lsm.accelData.y);       Serial.print(" ");
  Serial.print("Z: "); Serial.print((int)lsm.accelData.z);     Serial.print(" ");
  Serial.print("Mag X: "); Serial.print((int)lsm.magData.x);     Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)lsm.magData.y);         Serial.print(" ");
  Serial.print("Z: "); Serial.println((int)lsm.magData.z);       Serial.print(" ");
  digitalWrite(13,0);
  accelRDY=0;
  }
}

void LSM_RDY(void){
  accelRDY=1;
  digitalWrite(13,1);
 }
