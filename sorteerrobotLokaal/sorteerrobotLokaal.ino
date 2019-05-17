#include <Servo.h>

Servo servo1; //afval servo
Servo servo2; //kleuren servo

int LDRPin = A0; // analoge pin LDR

int huidigeStatus = 0; 
int vorigeStatus = 0;

boolean huidigeStatus1 = false;
boolean vorigeStatus1 = false;

String mijnString; // string die binnenkomt op seriele monitor
String getKleur; // string waar de kleur wordt opgeslagen
String getAantal; // string waar het aantal van een orderwordt opgeslagen

String kleurKleurenSensor; // kleur die de kleurensensor stuurt

int getAantalInt; // integer waar het aantal van een order wordt opgeslagen

int aantalRodeBlokjes = 0; // aantal getelde rode blokjes
int aantalGroeneBlokjes = 0; // " groen "
int aantalGeleBlokjes = 0; // " geel "

int waardeLDR; // waarde die de LDR geeft 

int servoPin1 = 10; // digitale pin waar de afval servo wordt aangesloten
int servoPin2 = 9; // " kleuren servo " 

int servoAngle1 = 0; // begin hoek afvalservo
int servoAngle2 = 120; // begin hoek kleurenservo

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  pinMode(LDRPin, INPUT); // initialiseren van de LDR pin

  pinMode(4, OUTPUT); // motor aanzetten
  pinMode(5, OUTPUT); // snelheid motor

}

//--------------BEGIN LOOP---------------
void loop() {
  waardeLDR = analogRead(LDRPin); // constant uitlezen LDR

// onderstaande if kijkt of er info op de seriele monitor aanwezig is
  if (Serial.available() > 0) { 
    mijnString = Serial.readString();
    Serial.println(mijnString);
    if (mijnString.equals("start")) { // bij start dan start de motor
      digitalWrite(4, LOW);
      analogWrite(5, 255);
    } else if (mijnString.equals("stop")) { //  bij stop dan stopt de motor
      analogWrite(5, 0);
    }
  }

  if (mijnString.indexOf(":") > 0) { // als de : in een string voorkomt dan komt hij hier in
    huidigeStatus1 = true; // wanneer op true, dan weten we dat er een order binnenkomt, want order komt binnen als bijv. (rood:3)
  } else {
    huidigeStatus1 = false; // wanneer op false is er geen order binnen gekomen, maar iets anders
  }

  if (huidigeStatus1) {
    getKleur = getValue(mijnString, ':', 0); // links van de : is de kleur en wordt in getKleur gestop
    getAantal = getValue(mijnString, ':', 1); // rechts van de : is het aantal blokjes wat gesorteerd moet worden
    huidigeStatus1 = false; // wordt weer op false gezet, zodat de order maar 1 keer binnekomt
  }

  int getAantalInt = getAantal.toInt(); // aantal wordt omgezet naar integer
  
  if (getKleur.equals("rood") && mijnString.startsWith("z") && aantalRodeBlokjes < getAantalInt)) { // de kleur van de order is rood, de kleur die de kleurensensor stuurt begint met z(rood) 
    // en het aantal getelde rode blokjes is kleiner dan het aantal in de order
    servoGoed(); // zet de afval servo zo dat hij het blokje door laat
    servoRood(); // zet de kleurenservo zo dat het blokje in het rode bakje komt
  } else if ((getKleur.equals("geel") && mijnString.startsWith("x") && aantalGeleBlokjes < getAantalInt)) {// de kleur van de order is geel, de kleur die de kleurensensor stuurt begint met x(geel) 
    // en het aantal getelde gele blokjes is kleiner dan het aantal in de order
    servoGoed(); 
    servoGeel();
  } else if ((getKleur.equals("groen") && mijnString.startsWith("y") && aantalGroeneBlokjes < getAantalInt)) {// de kleur van de order is groen, de kleur die de kleurensensor stuurt begint met y(groen) 
    // en het aantal getelede groene blokjes is kleiner dan het aantal in de order
    servoGoed();
    servoGroen();
  } else if (!mijnString.indexOf(getKleur) == 0) { // wanneer de data van de seriele monitor niet de kleur van de order bevat
    servoFout(); // servo wordt zo gezet dat het blokje in het afvalbakje komt
  }

  //wanneer de waarde onder de 400 komt dan wordt de huidigeStatus op 1 gezet
  if (waardeLDR < 400) {
    huidigeStatus = 1;
  } else { //wanneer de waarde anders is wordt de huidigeStatus op 0 gezet
    huidigeStatus = 0;
  }
  //wanneer de huidigeStatus 1 is en de vorigeStatus 0 is dan wordt er 1 geteld
  if (huidigeStatus == 1 && vorigeStatus == 0) {
    if (getKleur.equals("rood")) { // wanneer de orderkleur rood is komt hij hier in
      aantalRodeBlokjes++; // weet dat hij er een bij het aantalRodeBlokjes moet optellen
      Serial.println("qgeteld"); // stuurt naar de applicatie dat hij geteld is
      delay(30);
      Serial.println("mrood"); // stuurt mrood naar de applicatie, zodat er bij rood 1 opgeteld kan worden 
      delay(30);
      if (aantalRodeBlokjes >= getAantalInt) { // wanneer het aantal geteld is wat er in de order zit komt hij hier in
        servoFout(); // servo wordt zo gezet dat alles wat er nog langs komt in het afvalbakje wordt gegooid
        Serial.println("k"); // wordt geprint naar de applicatie, zodat de volgende order ingeladen kan worden. 
        huidigeStatus1 = true; // wordt op true gezet zodat arduino weet dat er een order is binnengekomen
        getKleur = ""; // get kleur wordt leeggehaald
      } // VOOR ONDERSTAANDE CODE GELDT HETZELFDE ALS HIERBOVEN BESCHREVEN MAAR DAN MET ANDERE KLEUR
    } else if (getKleur.equals("geel")) {
      aantalGeleBlokjes++;
      Serial.println("qgeteld");
      delay(30);
      Serial.println("pgeel");
      delay(30);
      if (aantalGeleBlokjes >= getAantalInt) {
        servoFout();
        Serial.println("k");
        huidigeStatus1 = true;
        getKleur = "";
      }
    }
    else if (getKleur.equals("groen")) {
      aantalGroeneBlokjes++;
      Serial.println("qgeteld");
      delay(30);
      Serial.println("vgroen");
      delay(30);
      if (aantalGroeneBlokjes >= getAantalInt) {
        servoFout();
        Serial.println("k");
        huidigeStatus1 = true;
        getKleur = "";
      }
    }
  }
  vorigeStatus = huidigeStatus; //de vorigeStatus wordt op de huidigeStatus gezet

}

// Functie om mijnString te splitten doormiddel van een karakter
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
  servo2.write(97);
}

void servoGroen() {
  servo2.write(40);
}
