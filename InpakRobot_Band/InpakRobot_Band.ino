void setup() {
    pinMode(4, OUTPUT);
    pinMode(7, OUTPUT);

    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
}

void loop() {
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);

    analogWrite(5, 255);
    analogWrite(6, 255);
}
