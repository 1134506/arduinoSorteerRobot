/*
Into Robotics
*/
 
#include <Servo.h>  //add '<' and '>' before and after servo.h
 
int servoPin = 9;
 
Servo servo;  
 
int servoAngle = 0;   // servo position in degrees
 
void setup()
{
  Serial.begin(9600);  
  servo.attach(servoPin);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}
 
 
void loop()
{
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);

    analogWrite(5, 255);
    analogWrite(6, 255);
     

  for(servoAngle = 45; servoAngle > 0; servoAngle--){
    servo.write(servoAngle);
    delay(3);
  }

 


  
  
}
