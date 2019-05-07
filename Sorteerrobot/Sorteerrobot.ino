#include <Wire.h>
#include <Servo.h>
#include <NewPing.h>

NewPing sonar(7,7,10);
Servo servo1; // afvalservo
Servo servo2; // kleurenservo

int huidigeStatus = 0;
int vorigeStatus = 0;

int aantalRodeBlokjes;
int aantalGroeneBlokjes;
int aantalGeleBlokjes;
int aantalBlokjes;

int servoPin1 = 9; // afvalservo pin
int servoPin2 = 10; // kleurenservo pin

int servoAngle1 = 0; // begin hoek afvalservo
int servoAngle2 = 120; // begin hoek kleurenservo

String getKleur;
String getAantal;

String mijnString;

void setup() {
  Serial.begin(9600);
  
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  pinMode(4, OUTPUT); // motor aanzetten
  pinMode(5, OUTPUT); // snelheid motor

  digitalWrite(4, HIGH);
  analogWrite(5, 255);

}

void loop() {
  if(Serial.available() > 0){
    mijnString = Serial.readString();
  }
  getKleur = getValue(mijnString, ':', 0); // deel van de string voor de ':'
  getAantal = getValue(mijnString, ':', 1); // deel van de string na de ':'
  int getAantalInt = stringToNumber(getAantal);
  Serial.println("Kleur: " + getKleur);
  Serial.println("Aantal: " + getAantal); 
  delay(1000);
}

void sorteerStand(String kleur, int aantal){
  while(getAantalInt > aantalBlokjes){
    if(getKleur.equals("rood")){
      servoGoed();
      servoRood();
    }else if(getKleur.equals("geel"){
      servoGoed();
      servoGeel();
    }else if(getKleur.equals("groen"){
      servoGoed();
      servoGroen();
    }
  }
  getKleur = "";
  getAantalInt = 0;
}

void servoGoed(){
  servo1.write(180);
}

void servoFout(){
  servo1.write(130);
}

void servoRood(){
  servo2.write(1);
}

void servoGeel(){
  for(servoAngle1 = 75; servoAngle1 < 95; servoAngle1++){
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

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
