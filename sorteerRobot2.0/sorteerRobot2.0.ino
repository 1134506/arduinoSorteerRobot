#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

int servoPin1 = 10;
int servoPin2 = 9;

Servo servo1;
Servo servo2;

int servoAngle1 = 0;
int servoAngle2 = 120;   // servo position in degrees

String kleur = "onbekend";
String input;

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

  digitalWrite(4, LOW);
  digitalWrite(7, LOW);

  analogWrite(5, 255);
  analogWrite(6, 255);

  // Now we're ready to get readings!
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
  } else if (kleur == "rood") {
    servoGoed();
  } else if (kleur == "groen") {
    servoGoed();
  } else if (kleur == "onbekend") {
    servoFout();
  }

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
