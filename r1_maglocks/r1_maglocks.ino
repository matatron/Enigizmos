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
  if (arcana.respuesta.substring(1, 4).equals("off")) {
    reset();
  }
  if (arcana.respuesta.substring(1, 3).equals("on")) {
    reset();
  }
  if (arcana.respuesta.toInt() == 0) {
    reset();
  }
  //if (respuesta.toInt() > 5) { }

  //electroiman de escudo de atena
  if (arcana.respuesta.toInt() >= 5) {
    digitalWrite(D0, LOW);
  }
  //maquina de humo
  if (arcana.respuesta.toInt() >= 1) {
    digitalWrite(D1, LOW);
  }
  //luz del cuarto
  if (arcana.respuesta.toInt() >= 12) {
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
  }
  //electroiman de la esfinge
  if (arcana.respuesta.toInt() >= 17) {
    digitalWrite(D5, LOW);
  }


  String estado = "";

  arcana.estado(estado);
  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);

  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);
}
