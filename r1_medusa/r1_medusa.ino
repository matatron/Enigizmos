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
    counter = 240;
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
  if (arcana.respuesta.substring(1, 4).equals("onn")) {
    reset();
    modo = 0;
  }
  //if (arcana.respuesta.toInt() >= 14) { }

  if (arcana.respuesta.toInt() == 14 && modo != 1) {
    modo = 1;
  }
  if (arcana.respuesta.toInt() == 15 && modo != 2) {
    modo = 2;
  }
  if (arcana.respuesta.toInt() == 16 && modo != 3) {
    modo = 3;
  }
  if (arcana.respuesta.toInt() >= 17 && modo < 4) {
    modo = 4;
  }

  switch (modo) {
    case 0:
      estado = "medusa=dormida";
      arcana.estado(estado);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D5, HIGH);
      digitalWrite(D6, HIGH);
      digitalWrite(TX, HIGH);
      digitalWrite(RX, HIGH);
      break;
    case 1:
        estado = "medusa=forma1";
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
          if (counter < 80) {
          digitalWrite(TX, LOW);
            digitalWrite(D2, LOW);
            digitalWrite(D6, HIGH);
            digitalWrite(D5, HIGH);
            digitalWrite(D3, LOW);
            digitalWrite(RX, LOW);
          } else if (counter < 160) {
          digitalWrite(TX, LOW);
            digitalWrite(D2, LOW);
            digitalWrite(D6, LOW);
            digitalWrite(D5, LOW);
            digitalWrite(D3, HIGH);
            digitalWrite(RX, HIGH);
          } else {
            digitalWrite(TX, HIGH);
            digitalWrite(D2, HIGH);
            digitalWrite(D6, LOW);
            digitalWrite(D5, LOW);
            digitalWrite(D3, LOW);
            digitalWrite(RX, LOW);
          }
      break;
    case 3:
        estado = "medusa=forma3";
        arcana.estado(estado);
        if (counter < 120) {
        digitalWrite(TX, HIGH);
          digitalWrite(D2, LOW);
          digitalWrite(D6, HIGH);
          digitalWrite(D5, LOW);
          digitalWrite(D3, HIGH);
          digitalWrite(RX, LOW);
        } else {
          digitalWrite(TX, LOW);
          digitalWrite(D2, HIGH);
          digitalWrite(D6, LOW);
          digitalWrite(D5, HIGH);
          digitalWrite(D3, LOW);
          digitalWrite(RX, HIGH);
        }


      break;
    case 4:
        reset();
        estado = "medusa=derrotada";
        arcana.estado(estado);
        digitalWrite(D2, HIGH);
        digitalWrite(D3, HIGH);
        digitalWrite(D5, HIGH);
        digitalWrite(D6, HIGH);
        digitalWrite(TX, HIGH);
        digitalWrite(RX, HIGH);
        break;
      }


  if (digitalRead(D0)) {
    counter0++;
    if (counter0 > 10) counter0 = 8;
  } else {
    counter0--;
    if (counter0 < 0) counter0 = 0;
  }

  if (digitalRead(D1)) {
    counter1++;
    if (counter1 > 10) counter1 = 8;
  } else {
    counter1--;
    if (counter1 < 0) counter1 = 0;
  }


  if (counter0 >= 2 && counter1 >= 2 && modo > 0 && modo < 4) {
    modo ++;

    switch (modo) {
      case 0:
        estado = "medusa=dormida";
        arcana.estado(estado);
        break;
      case 1:
        estado = "medusa=forma1";
        arcana.estado(estado);
        break;
      case 2:
        estado = "medusa=forma2";
        arcana.estado(estado);
        break;
      case 3:
        estado = "medusa=forma3";
        arcana.estado(estado);
        break;
      case 4:
        estado = "medusa=derrotada";
        arcana.estado(estado);
        break;
    }

    arcana.conectar();
    reset();
    delay(500);
    animate();
  }


  ///////////////////////////////
  delay(25);
}

void reset() {
  counter1 = 0;
  counter0 = 0;
  digitalWrite(TX, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(RX, HIGH);
}

void animate() {
  digitalWrite(TX, LOW);
  delay(200);
  digitalWrite(D2, LOW);
  delay(200);
  digitalWrite(D6, LOW);
  delay(200);
  digitalWrite(D5, LOW);
  delay(200);
  digitalWrite(D3, LOW);
  delay(200);
  digitalWrite(RX, LOW);
  delay(200);
  digitalWrite(TX, HIGH);
  delay(200);
  digitalWrite(D2, HIGH);
  delay(200);
  digitalWrite(D6, HIGH);
  delay(200);
  digitalWrite(D5, HIGH);
  delay(200);
  digitalWrite(D3, HIGH);
  delay(200);
  digitalWrite(RX, HIGH);
}
