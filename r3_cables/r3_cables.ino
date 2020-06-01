#include <Arcana3.h>

Arcana3 arcana("cables", true);

int counter = 0;
bool isReset = false;
byte i = 0;

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, INPUT_PULLUP);
  Serial.begin(115200);
  reset();
  arcana.init();
  delay(50);
  arcana.configurar();
  delay(50);
}

void loop() {
  String valores = "";
  if (counter == 0) {
    arcana.conectar();
    counter = 50;

    if (arcana.status == 0 && !isReset) {
    }
    if (arcana.status == 1) {
    }
    if (arcana.status == 3 && !isReset) {
    }

  }
  counter--;

  if (arcana.status == 2) {
    isReset = false;

    for (i = 0; i < 12; i++) {
      setPos(i);
      valores += (digitalRead(D6)) ? String("0") : String("1");

      if (i%3 == 2 && i<10) {
        valores += String(" ");
      }
    }

  }

  String estado = "";
  estado += "&Valores=" + valores;
  arcana.estado(estado);


  ///////////////////////////////
  delay(20);
}

void setPos(int n) {
  digitalWrite(D1, bitRead(n, 0));
  digitalWrite(D2, bitRead(n, 1));
  digitalWrite(D3, bitRead(n, 2));
  digitalWrite(D5, bitRead(n, 3));
  delay(5);
}

void reset() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D5, LOW);
  isReset = true;
}
