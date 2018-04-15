// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot
void setup() {
  Serial.begin(115200); 
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);            
  Serial.print("s1:" );                       
  Serial.println(sensorValue);      
  delay(2);                     
}
