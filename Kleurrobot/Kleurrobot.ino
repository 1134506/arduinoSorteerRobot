#include <Wire.h>


//de kleur die de sensor ziet is standaard onbekend
String kleurKleurenSensor;

String mijnString;

//initaliseren van de rgb sensor

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  
   if (Serial.available() > 0) {
    mijnString = Serial.readString();
      if (mijnString.equals("start")) {
        digitalWrite(4, HIGH);
        analogWrite(5, 255);
    } else if (mijnString.equals("stop")) {
      analogWrite(5, 0);
    }
  }

}
