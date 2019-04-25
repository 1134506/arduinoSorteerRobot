#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Now we're ready to get readings!
}

void loop(void) {
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  if (r > g && g > (b * 2)) {
    Serial.println("ik denk dat dit geel is");
  } else if (r > (g * 4) && r > (b * 4)) {
    Serial.println("ik denk dat dit rood is");
  } else if (g > (r * 1.5) && g > (b * 2)) {
    Serial.println("ik denk dat dit groen is");
  } else if (b > (r * 1.5) && b > g) {
    Serial.println("ik denk dat dit blauw is");
  } else if (r > 130 && g > 130 && b > 130) {
    Serial.println("ik denk dat dit een onbekende kleur is");
  }
}
