#include <Arcana.h>

int counter = 0;
Arcana arcana("gizmo6", true);

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);

  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

  reset();

  Serial.begin(115200);
  delay(10);
  arcana.init();
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50;
  }
  counter--;
  ///////////////////////////////
  if (arcana.respuesta.toInt() == 0) {
    reset();
  }
  //if (respuesta.toInt() > 5) { }

  //maquina de humo
  if (arcana.respuesta.toInt() >= 8) {
    digitalWrite(D0, HIGH);
  }
  //luz del cuarto
  if (arcana.respuesta.toInt() >= 12) {
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
  }

  //electroiman de la esfinge
  if (arcana.respuesta.toInt() >= 6) {
    digitalWrite(D3, HIGH);
  }

  //electroiman de la esfinge
  if (arcana.respuesta.toInt() >= 15) {
    digitalWrite(D6, HIGH);
  }
  
  //electroiman de la puerta
  if (arcana.respuesta.toInt() >= 16) {
    digitalWrite(D7, HIGH);
  }

  String estado = "respuesta=" + arcana.respuesta.substring(1, 2);

  arcana.estado(estado);
  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);

  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
}
