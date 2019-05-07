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
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);

    analogWrite(5, 255);
    analogWrite(6, 255);
     

  servo.write(0);
  delay(700);
  servo.write(179);
  delay(700);


 
//hallo

  
  
}
