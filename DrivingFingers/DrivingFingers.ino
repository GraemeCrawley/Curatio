#include <Wire.h>

int const numFingers = 4;
int analogPins[numFingers] = {
  A3, A2, A1, A0};
int fingerOpen[numFingers];
int fingerClose[numFingers];
  
  
void countdown(int secs){
  for(int i=secs;i>0;i--){
    Serial.print(i);
    Serial.println("...");
    delay(1000);
  }
}
  
void setup(){
  Serial.begin(9600); 

  Serial.println("Sensing open position in... ");
  countdown(3);
  for (int i=0;i<numFingers;i++){
    fingerOpen[i]=analogRead(analogPins[i]);
  }
  Serial.println("Done ");

  delay(500);

  Serial.println("Sensing close position in... ");
  countdown(5);
  for (int i=0;i<numFingers;i++){
    fingerClose[i]=analogRead(analogPins[i]);
  }
  Serial.println("Done");
  delay(500);
  
  Wire.begin(); 
}

void loop(){
  Wire.beginTransmission(2);
  Wire.write(-1);
 for(int i = 0; i < numFingers; i++){
   Wire.write(map(analogRead(analogPins[i]),fingerOpen[i],fingerClose[i],0,255));
 }
 Wire.endTransmission();
 delay(100);
}
