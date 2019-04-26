#include <NewPing.h>
NewPing sonar(7,7,10);

int aantalDozen = 0;
int currentState = 0;
int previousState = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
//wanneer de afstand tussen de 7 en 0 cm is dan wordt de currentstate op 1 gezet 
  if(sonar.ping_cm() <= 7 && sonar.ping_cm() > 0){
    currentState = 1;
//wanneer de afstand anders is wordt de currentstate op 0 gezet
  }else{
    currentState = 0;
  }

//wanneer de currentstate 1 is en de previousstate 0 is dan wordt er 1 geteld
  if(currentState == 1 && previousState == 0){
    aantalDozen++;
    delay(5000);
  }
//de previousstate wordt op de currentstate gezet
  previousState = currentState;
  
  Serial.println(aantalDozen);
  //Serial.println(sonar.ping_cm());
 
}
