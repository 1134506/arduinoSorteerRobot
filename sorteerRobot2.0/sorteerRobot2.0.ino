#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#include <NewPing.h>

NewPing sonar(7,7,10);
Servo servo1; // afvalservo
Servo servo2; // kleurenservo


int currentState = 0;
int previousState = 0;

int aantalRoden;
int aantalGroenen;
int aantalGelen;

int servoPin1 = 10;
int servoPin2 = 9;

int servoAngle1 = 0;  //Servo afvalbak
int servoAngle2 = 120;   //Servo voor de kleuren

String kleur = "onbekend";
String input;

// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);


void setup(void) {
  Serial.begin(9600);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  //Motor voor de sorteerband
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);

  analogWrite(5, 255);
  analogWrite(6, 255);
}

void loop(void) {
  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
  
  if (r > (b * 3) && g > (b * 2.5)) {
    kleur = "geel";
  } else if (r > (g * 4) && r > (b * 4)) {
    kleur = "rood";
  } else if (g > r && g > (b * 2)) {
    kleur = "groen";
  } else if (r > 130 && g > 130 && b > 130) {
    kleur = "onbekend";
  }

  if (kleur == "geel") {
    servoGoed();
    while (kleur == "geel") {
    servoGeel();
    if (Serial.available() > 0) {
      input = Serial.readString();
    }
    if (input == "geteld") {
      kleur = "onbekend";
      input = "";
    }
  }
  } else if (kleur == "rood") {
    servoGoed();
    while (kleur == "rood") {
    servoRood();
    if (Serial.available() > 0) {
      input = Serial.readString();
    }
    if (input == "geteld") {
      kleur = "onbekend";
      input = "";
    }
  }
  } else if (kleur == "groen") {
    servoGoed();
    while (kleur == "groen") {
    servoGroen();
    if (Serial.available() > 0) {
      input = Serial.readString();
    }
    if (input == "geteld") {
      kleur = "onbekend";
      input = "";
    }
  }
  } else if (kleur == "onbekend") {
    servoFout();
  }

  //wanneer de afstand tussen de 7 en 0 cm is dan wordt de currentstate op 1 gezet 
  if(sonar.ping_cm() <= 7 && sonar.ping_cm() > 0){
    currentState = 1;
  //wanneer de afstand anders is wordt de currentstate op 0 gezet
  }else{
    currentState = 0;
  }

  //wanneer de currentstate 1 is en de previousstate 0 is dan wordt er 1 geteld
  if(currentState == 1 && previousState == 0){
    input = "geteld";
    delay(5000);
  }
  //de previousstate wordt op de currentstate gezet
  previousState = currentState;
  
  Serial.println(aantalDozen);
}

void servoFout() {
  servo1.write(130);
}

void servoGoed() {
  servo1.write(180);
}

void servoRood() {
  servo2.write(1);
}

void servoGeel() {
  for (servoAngle1 = 75; servoAngle1 < 95; servoAngle1++) {
    servo2.write(servoAngle1);
    delay(5);
  }
}

void servoGroen() {
  for (servoAngle1 = 60; servoAngle1 > 20; servoAngle1--) {
    servo2.write(servoAngle1);
    delay(5);
  }
}
