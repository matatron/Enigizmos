#include <Arcana3.h>

Arcana3 arcana("gps", true);

int counter = 0;
bool isReset = false;
bool isDone = false;
unsigned long timer1 = 0;
unsigned long timer2 = 0;
unsigned long timer3 = 0;
bool bola1 = false;
bool bola2 = false;
bool bola3 = false;

void setup() {
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, OUTPUT);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  reset();
  arcana.init();
  delay(50);
}

void loop() {
  String valores = "";
  if (counter == 0) {
    arcana.conectar();
    counter = 50;

    if (arcana.status == 0) {
      reset();
    }
    if (arcana.status == 1) {
    }
    if (arcana.status == 3) {
      reset();
    }

  }
  counter--;

  if (arcana.status == 2) {

    if (digitalRead(D3) == LOW) {
      bola1 = true;
      timer1 = millis();
    } else {
      if (millis() > timer1 + 1000) {
        bola1 = false;
      }
    }
    if (digitalRead(D5) == LOW) {
      bola2 = true;
      timer2 = millis();
    } else {
      if (millis() > timer2 + 1000) {
        bola2 = false;
      }
    }
    if (digitalRead(D6) == LOW) {
      bola3 = true;
      timer3 = millis();
    } else {
      if (millis() > timer3 + 1000) {
        bola3 = false;
      }
    }

    if (bola1 && bola2 && bola3) {
      isDone = true;
      isReset = false;
    }
  }

  String estado = "";
  estado += "&B1=" + (bola1 ? String("si") : String("no"));
  estado += "&B2=" + (bola2 ? String("si") : String("no"));
  estado += "&B3=" + (bola3 ? String("si") : String("no"));
  estado += "&GPS=" + (isDone ? String("encendido") : String("apagado"));
  arcana.estado(estado);


  ///////////////////////////////
  delay(20);
}

void reset() {
  bola1 = false;
  bola2 = false;
  bola3 = false;
  isReset = true;
  isDone = false;
}
