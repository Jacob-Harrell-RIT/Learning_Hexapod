// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot
int n=0;
void setup() {
  Serial.begin(9600); 
}

void loop(){
  // read the analog in value:
  for(n=0;n<=5;n++){
    sensorValue = analogRead(analogInPin+n);            
    Serial.print("s" );  
    Serial.print(n);
    Serial.print(':');
    Serial.print(sensorValue);   
    Serial.print(",");
  }
  Serial.println();                        
}
