#include <Arcana.h>

#define S0 15
#define S1 3
#define S2 1
#define none 9

int counter = 0;
Arcana arcana("hidra", false);

int LED[] = {16, 5, 4, 0, 2, 14, 12, 13};
int light[] = {1, 1, 1, 1, 1, 1, 1};
int starting[] = {1, 1, 1, 1, 1, 1, 1};
int i;
int currentButton = none;
int lastButton = none;
bool isReset = false;
bool isWon = false;
int modo = 0;


void setup() {
  for (i = 0; i < 7; i ++) {
    pinMode(LED[i], OUTPUT);
  }

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  reset();

  //  Serial.begin(115200);
  delay(10);
  arcana.init();
}
void loop() {
  String estado;

  if (modo == 0) {
    estado = "modo=dormida";
    arcana.estado(estado);

    if (counter == 0) {
      arcana.conectar();
      counter = 50;
    }
    counter--;

      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], LOW);
      }

    if (arcana.respuesta.toInt() == 0 && !isReset) {
      reset();
    }
    if (arcana.respuesta.substring(1, 4).equals("off") && !isReset) {
      reset();
    }

    if (arcana.respuesta.toInt() >= 13) {
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], HIGH);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], LOW);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], HIGH);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], LOW);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], HIGH);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], LOW);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], HIGH);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], LOW);
      }

      modo =  1;
      isReset = false;
    }

  }

  if (modo == 1) {
    estado = "modo=despierta";
    arcana.estado(estado);


    currentButton = none;
    for (i = 0; i < 8; i ++) {
      digitalWrite(S0, bitRead(i, 0));
      digitalWrite(S1, bitRead(i, 1));
      digitalWrite(S2, bitRead(i, 2));
      delay(1);
      //Serial.print((analogRead(A0) > 512));

      if (analogRead(A0) > 512) currentButton = i;
    }

    if (currentButton != lastButton) {
      lastButton = currentButton;

      if (currentButton != none) {

        switch (currentButton) {
          case 0:
            light[currentButton] = 1 - light[currentButton];
            light[6] = 1 - light[6];
            break;
          case 1:
            light[currentButton] = 1 - light[currentButton];
            light[4] = 1 - light[4];
            light[5] = 1 - light[5];
            break;
          case 2:
            light[currentButton] = 1 - light[currentButton];
            light[3] = 1 - light[3];
            break;
          case 3:
            light[2] = 1 - light[2];
            light[currentButton] = 1 - light[currentButton];
            break;
          case 4:
            light[currentButton] = 1 - light[currentButton];
            light[1] = 1 - light[1];
            light[6] = 1 - light[6];
            break;
          case 5:
            light[currentButton] = 1 - light[currentButton];
            light[0] = 1 - light[0];
            light[2] = 1 - light[2];
            break;
          case 6:
            light[currentButton] = 1 - light[currentButton];
            light[4] = 1 - light[4];
            break;
          case 7:
            light[2] = 1 - light[2];
            light[3] = 1 - light[3];
            break;
        }
      }
    }

    isWon = true;
    for (i = 0; i < 7; i ++) {
      if (light[i] == 1) isWon = false;
      digitalWrite(LED[i], light[i]);
    }

    if (isWon) {
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], HIGH);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], LOW);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], HIGH);
      }
      delay(400);
      for (i = 0; i < 7; i ++) {
        digitalWrite(LED[i], LOW);
      }
      modo = 2;
    }

  }
  if (modo == 2) {
    estado = "modo=derrotada";
    arcana.estado(estado);

    if (counter == 0) {
      arcana.conectar();
      counter = 50;
    }
    counter--;

    if (arcana.respuesta.toInt() == 0 && !isReset) {
      reset();
    }
    if (arcana.respuesta.substring(1, 4).equals("off") && !isReset) {
      reset();
    }

  }

  delay(20);
}

void reset() {
  for (i = 0; i < 7; i ++) {
    light[i] = starting[i];
    digitalWrite(LED[i], LOW);
  }
  modo = 0;
  isReset = true;
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
}
