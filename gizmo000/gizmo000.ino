#include <Arcana.h>

int counter = 0;
Arcana arcana("gizmo0", true);

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


  String estado = "respuesta=" + arcana.respuesta.substring(1, 2);

  arcana.estado(estado);
  ///////////////////////////////
  delay(20);
}

void reset() {
}
