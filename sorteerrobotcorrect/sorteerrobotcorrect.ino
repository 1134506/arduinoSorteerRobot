#include <Servo.h>
#include "Adafruit_TCS34725.h"

Servo servo1; //afval servo
Servo servo2; //kleuren servo

int LDRPin = A0;
int huidigeStatus = 0;
int vorigeStatus = 0;

String mijnString;
String getKleur;
String getAantal;

String kleurKleurenSensor;

int aantalRodeBlokjes;
int aantalGroeneBlokjes;
int aantalGeleBlokjes;

int getAantalInt;

int waardeLDR;

int servoPin1 = 10;
int servoPin2 = 9;

int servoAngle1 = 0; // begin hoek afvalservo
int servoAngle2 = 120; // begin hoek kleurenservo

//initaliseren van de kleur sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (tcs.begin()) {
  } else {
    while (1);
  }


  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  pinMode(LDRPin, INPUT);

  pinMode(4, OUTPUT); // motor aanzetten
  pinMode(5, OUTPUT); // snelheid motor

}

//--------------BEGIN LOOP---------------
void loop() {
  uint16_t r, g, b, c, colorTemp, lux;
  waardeLDR = analogRead(LDRPin);

  if (Serial.available() > 0) {
    mijnString = Serial.readString();
    Serial.println(mijnString);
    if (mijnString.equals("start")) {
      digitalWrite(4, LOW);
      analogWrite(5, 255);
    } else if (mijnString.equals("stop")) {
      analogWrite(5, 0);
    }
  }

  getKleur = getValue(mijnString, ':', 0);
  getAantal = getValue(mijnString, ':', 1);

  int getAantalInt = getAantal.toInt();

  tcs.getRawData(&r, &g, &b, &c);

  if (r > (b * 3) && g > (b * 2.5)) {
    kleurKleurenSensor = "geel"; // 3 is geel
    Serial.println("geel");
  } else if (r > (g * 4) && r > (b * 4)) {
    kleurKleurenSensor = "rood";  // 1 is rood
  } else if (g > r && g > (b * 2)) {
    kleurKleurenSensor = "groen"; // 2 is groen
  } else if (r > 130 && g > 130 && b > 130) {
    kleurKleurenSensor = "onbekend"; // 0 is afval
  }

  if (getKleur.equals("rood") && getKleur.equals(kleurKleurenSensor) && aantalRodeBlokjes < getAantalInt) {
    servoGoed();
    servoRood();
  } else if (getKleur.equals("geel") && getKleur.equals(kleurKleurenSensor) && aantalGeleBlokjes < getAantalInt) {
    servoGoed();
    servoGeel();
  } else if (getKleur.equals("groen") && getKleur.equals(kleurKleurenSensor) && aantalGroeneBlokjes < getAantalInt) {
    servoGoed();
    servoGroen();
  } else {
    servoFout();
  }

  //wanneer de waarde onder de 400 komt dan wordt de huidigeStatus op 1 gezet
  if (waardeLDR < 400) {
    huidigeStatus = 1;
  } else { //wanneer de waarde anders is wordt de huidigeStatus op 0 gezet
    huidigeStatus = 0;
  }
  //wanneer de huidigeStatus 1 is en de vorigeStatus 0 is dan wordt er 1 geteld
  if (huidigeStatus == 1 && vorigeStatus == 0) {
    if (getKleur.equals("rood")) {
      aantalRodeBlokjes++;
      if (aantalRodeBlokjes >= getAantalInt) {
        servoFout();
        Serial.println("n");
        getKleur = "";
      }
    } else if (getKleur.equals("geel")) {
      aantalGeleBlokjes++;
      if (aantalGeleBlokjes >= getAantalInt) {
        servoFout();
        Serial.println("n");
        getKleur = "";
      }
    }
    else if (getKleur.equals("groen")) {
      aantalGroeneBlokjes++;
      if (aantalGroeneBlokjes >= getAantalInt) {
        servoFout();
        Serial.println("n");
        getKleur = "";
      }
    }
  }
  vorigeStatus = huidigeStatus; //de vorigeStatus wordt op de huidigeStatus gezet

}
//-------------EINDE LOOP--------------------

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
void servoGoed() {
  servo1.write(180);
}

void servoFout() {
  servo1.write(127);
}

void servoRood() {
  servo2.write(0);
}

void servoGeel() {
  for(int servoAngle = 100; servoAngle > 75; servoAngle--){
    servo2.write(servoAngle);
    delay(5);
  }
}

void servoGroen() {
  servo2.write(40);
}
