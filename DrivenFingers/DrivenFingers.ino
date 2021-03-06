#include <Servo.h> 
#include <Wire.h>

int const numFingers = 4;
int analogPins[numFingers] = {
  A3, A2, A1, A0};
int fingerOpen[numFingers];
int fingerClose[numFingers];
int fingerSetPoints[numFingers] = {
  127, 127, 127, 127};
int relativePos[numFingers];

int servoPins[numFingers] = { 
  11,9,6,5};
  //Index - 11
  //Middle - 9
  //Ring - 6
  //Pinky - 5
  
  //Thumb - 10
  
int servoOrientation[numFingers] = { 
  -1,1,-1,-1};
Servo servos[numFingers];

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void countdown(int secs){
  for(int i=secs;i>0;i--){
    Serial.print(i);
    Serial.println("...");
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600); 

  for (int i=0;i<numFingers;i++){
    servos[i].attach(servoPins[i]);
    servos[i].write(90);                 
  }
  
  Serial.println("Waiting for input to begin.");

  Serial.flush(); //flush all previous received and transmitted data
  while(!Serial.available()) ;

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
  Serial.flush();
}

boolean shouldRun = false;

void loop(){
  
  int deadbandRadius=5;
  for (int i=0;i<numFingers;i++){
    relativePos[i] = map(analogRead(analogPins[i]),fingerOpen[i],fingerClose[i],0,255);
    
    int error = abs(relativePos[i] - fingerSetPoints[i]);
    double kP = 120.0/255.0;
    
    double pVal = error * kP;
    if(pVal > 90) pVal = 90;
    
    if(shouldRun){
      if (abs(fingerSetPoints[i]-relativePos[i])<deadbandRadius){
        servos[i].write(90);
      }
      else if (fingerSetPoints[i]-relativePos[i]>0){
        servos[i].write(90+(pVal)*servoOrientation[i]);
      }
      else if (fingerSetPoints[i]-relativePos[i]<0){
        servos[i].write(90-(pVal)*servoOrientation[i]);
      }
    }
  }
  
  if(Serial.available()){
    char input = Serial.read();
    if(input == ('s')){
      shouldRun = false; 
    }
    if(input == ('g')){
      Serial.println("Grabbing");
      for(int i = 0; i < numFingers; i++) fingerSetPoints[i] = 200;
      shouldRun = true;
    }
    if(input == ('l')){
      Serial.println("Letting Go");
      for(int i = 0; i < numFingers; i++) fingerSetPoints[i] = 10;
      shouldRun = true;
    }
    if(input = ('p')){
      Serial.println("Party On");
      fingerSetPoints[0] = 10;
      fingerSetPoints[1] = 200;
      fingerSetPoints[2] = 200;
      fingerSetPoints[3] = 10;
    }
    if(input == ('f')){
      Serial.println("Being Fancy");
      fingerSetPoints[0] = 200;
      fingerSetPoints[1] = 200;
      fingerSetPoints[2] = 200;
      fingerSetPoints[3] = 10;
    }
    if(input == ('p')){
      Serial.println("Picking Nose");
      fingerSetPoints[0] = 10;
      fingerSetPoints[1] = 200;
      fingerSetPoints[2] = 200;
      fingerSetPoints[3] = 200;
    }
    Serial.flush();
  }
}




