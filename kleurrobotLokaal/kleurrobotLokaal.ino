#include "Adafruit_TCS34725.h"

//de kleur die de sensor ziet is standaard onbekend
String kleurKleurenSensor;

//de string die wordt gelezen door de seriele monitor
String mijnString;

//initaliseren van de rgb sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() {
  Serial.begin(9600);
}

void loop() {
  //uint is een unassigned integer met de namen: r,g,b,c,colorTemp en lux
  uint16_t r, g, b, c, colorTemp, lux;
  
  //wanneer er iets beschikbaar is in de seriele monitor, dan komt hij in de if
  if (Serial.available() > 0) {
    mijnString = Serial.readString();
    if (mijnString.equals("start")) { // wanneer er start wordt geprint gaat de motor aan
      digitalWrite(4, HIGH);
      analogWrite(5, 255);
    } else if (mijnString.equals("stop")) { // bij stop gaat hij uit
      analogWrite(5, 0);
    }
  }

  tcs.getRawData(&r, &g, &b, &c); // de kleursensor wordt uitgelezen

  if (r > (b * 3) && g > (b * 2.5)) { // de waarde van geel
    kleurKleurenSensor = "xgeel"; // 3 is geel
    Serial.println(kleurKleurenSensor); // print xgeel
  } else if (r > (g * 4) && r > (b * 4)) { // de waarde van rood
    kleurKleurenSensor = "zrood";  // 1 is rood
    Serial.println(kleurKleurenSensor); // print zrood
  } else if (g > r && g > (b * 2)) { // de waarde van groen
    kleurKleurenSensor = "ygroen"; // 2 is groen
    Serial.println(kleurKleurenSensor); // print ygroen
  }
}
