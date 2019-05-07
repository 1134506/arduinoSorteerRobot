#include <Wire.h>
#include "Adafruit_TCS34725.h"

//de kleur die de sensor ziet is standaard onbekend
String kleur = "onbekend";
String input;

//initaliseren van de rgb sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() {
  Serial.begin(9600);

  if(tcs.begin()){
    Serial.println("Found sensor");
  }else{
    Serial.println("No sensor found");
    while(1);
  }

  pinMode(4, OUTPUT); //Motor aandoen
  pinMode(5, OUTPUT); //Snelheid motor
  
  //Motor voor de sorteerband
  digitalWrite(4, HIGH);
  analogWrite(5, 255);

}

void loop() {
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);

  if (r > (b * 3) && g > (b * 2.5)) {
    kleur = "geel";
  } else if (r > (g * 4) && r > (b * 4)) {
    kleur = "rood";
  } else if (g > r && g > (b * 2)) {
    kleur = "groen";
  } else if (r > 130 && g > 130 && b > 130) {
    kleur = "onbekend";
  }
  
  Serial.println(kleur);

}
