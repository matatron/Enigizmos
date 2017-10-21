#include <Arcana.h>
#define TX 1
#define RX 3

int counter = 0;
int counter0 = 0;
int counter1 = 0;
Arcana arcana("medusa", true);

int modo = 0;

void setup() {
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);

  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(TX, OUTPUT);
  pinMode(RX, OUTPUT);

  reset();

  //Serial.begin(115200);
  delay(10);
  arcana.init();
  delay(10);
}
void loop() {
  String estado = "";
  if (counter == 0) {
    arcana.conectar();
    counter = 200;
  }
  counter--;
  ///////////////////////////////
  if (arcana.respuesta.toInt() == 0) {
    reset();
    modo = 0;
  }
  if (arcana.respuesta.substring(1, 4).equals("off")) {
    reset();
    modo = 0;
  }
  //if (arcana.respuesta.toInt() >= 14) { }

  if (arcana.respuesta.toInt() >= 14 && modo == 0) {
    modo = 1;
  }

  switch (modo) {
    case 0:
      estado = "medusa=dormida&progreso="+String(arcana.respuesta.toInt());
      arcana.estado(estado);
      break;
    case 1:
      estado = "medusa=despierta";
      arcana.estado(estado);
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
      digitalWrite(D5, LOW);
      digitalWrite(D6, LOW);
      digitalWrite(TX, LOW);
      digitalWrite(RX, LOW);
      break;
    case 2:
      estado = "medusa=forma2";
      arcana.estado(estado);
      if (counter < 100) {
        digitalWrite(D2, HIGH);
        digitalWrite(TX, LOW);
        digitalWrite(D6, HIGH);
        digitalWrite(D5, LOW);
        digitalWrite(D3, HIGH);
        digitalWrite(RX, HIGH);
      } else {
        digitalWrite(D2, HIGH);
        digitalWrite(TX, HIGH);
        digitalWrite(D6, LOW);
        digitalWrite(D5, HIGH);
        digitalWrite(D3, LOW);
        digitalWrite(RX, HIGH);
      }
      break;
    case 3:
      estado = "medusa=forma3";
      arcana.estado(estado);
      if (counter < 66) {
        digitalWrite(D2, LOW);
        digitalWrite(TX, LOW);
        digitalWrite(D6, HIGH);
        digitalWrite(D5, HIGH);
        digitalWrite(D3, LOW);
        digitalWrite(RX, LOW);
      } else if (counter < 133) {
        digitalWrite(D2, LOW);
        digitalWrite(TX, LOW);
        digitalWrite(D6, LOW);
        digitalWrite(D5, LOW);
        digitalWrite(D3, HIGH);
        digitalWrite(RX, HIGH);
      } else {
        digitalWrite(D2, HIGH);
        digitalWrite(TX, HIGH);
        digitalWrite(D6, LOW);
        digitalWrite(D5, LOW);
        digitalWrite(D3, LOW);
        digitalWrite(RX, LOW);
      }
      break;
    case 4:
      reset();
      estado = "medusa=derrotada";
      arcana.estado(estado);
      break;
  }


  if (digitalRead(D0)) {
    counter0++;
    if (counter0 > 6) counter0 = 6;
  } else {
    counter0--;
    if (counter0 < 0) counter0 = 0;
  }

  if (digitalRead(D1)) {
    counter1++;
    if (counter1 > 6) counter1 = 6;
  } else {
    counter1--;
    if (counter1 < 0) counter1 = 0;
  }


  if (counter0 >= 3 && counter1 >= 3 && modo < 4) {
    modo ++;
    reset();
    animate();

  }


  ///////////////////////////////
  delay(25);
}

void reset() {
  digitalWrite(D2, HIGH);
  digitalWrite(TX, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(RX, HIGH);
}

void animate() {
  digitalWrite(D2, LOW);
  delay(200);
  digitalWrite(D2, HIGH);
  digitalWrite(TX, LOW);
  delay(200);
  digitalWrite(TX, HIGH);
  digitalWrite(D6, LOW);
  delay(200);
  digitalWrite(D6, HIGH);
  digitalWrite(D5, LOW);
  delay(200);
  digitalWrite(D5, HIGH);
  digitalWrite(D3, LOW);
  delay(200);
  digitalWrite(D3, HIGH);
  digitalWrite(RX, LOW);
  delay(200);
  digitalWrite(RX, HIGH);
}
