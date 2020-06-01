unsigned int i = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D2, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

    for (i = 0; i < 12; i++) {
      setPos(i);
      Serial.print(digitalRead(D2));
      Serial.println("");
    }

}

void setPos(int n) {
  digitalWrite(D7, bitRead(n, 0));
  digitalWrite(D6, bitRead(n, 1));
  digitalWrite(D5, bitRead(n, 2));
  digitalWrite(D3, bitRead(n, 3));
  delay(5);
}
