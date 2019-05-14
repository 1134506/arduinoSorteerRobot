#include <Wire.h>
#include "Adafruit_TCS34725.h"

//de kleur die de sensor ziet is standaard onbekend
String kleurKleurenSensor;


String mijnString;

//initaliseren van de rgb sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint16_t r, g, b, c, colorTemp, lux;

  if (Serial.available() > 0) {
    mijnString = Serial.readString();
    if (mijnString.equals("start")) {
      digitalWrite(4, HIGH);
      analogWrite(5, 255);
    } else if (mijnString.equals("stop")) {
      analogWrite(5, 0);
    }
  }

  tcs.getRawData(&r, &g, &b, &c);

  if (r > (b * 3) && g > (b * 2.5)) {
    kleurKleurenSensor = "xgeel"; // 3 is geel
    Serial.println(kleurKleurenSensor);
  } else if (r > (g * 4) && r > (b * 4)) {
    kleurKleurenSensor = "zrood";  // 1 is rood
    Serial.println(kleurKleurenSensor);
  } else if (g > r && g > (b * 2)) {
    kleurKleurenSensor = "ygroen"; // 2 is groen
    Serial.println(kleurKleurenSensor);
  } else if (r > 130 && g > 130 && b > 130) {
    kleurKleurenSensor = "onbekend"; // 0 is afval
    Serial.println(kleurKleurenSensor);
  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
