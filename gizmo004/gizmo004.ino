#include <Arcana.h>

#define S0 15
#define S1 3
#define S2 1
#define none 9

int counter = 0;
Arcana arcana("gizmo4", true);

int LED[] = {16, 5, 4, 0, 2, 14, 12, 13};
int light[] = {1, 1, 1, 1, 1, 1, 1, 1};
int i;
int currentButton = none;
int lastButton = none;


void setup() {
  for (i = 0; i < 8; i ++) {
    pinMode(LED[i], OUTPUT);
  }

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

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

  currentButton = none;
  for (i = 0; i < 8; i ++) {
    digitalWrite(S0, bitRead(i, 0));
    digitalWrite(S1, bitRead(i, 1));
    digitalWrite(S2, bitRead(i, 2));
    //    Serial.print(digitalRead(SIG));

    if (analogRead(A0) > 512) currentButton = i;
  }

  if (currentButton != lastButton) {
    lastButton = currentButton;

    if (currentButton != none) {
      light[currentButton] = 1 - light[currentButton];
    }
  }

  for (i = 0; i < 8; i ++) {
    digitalWrite(LED[i], light[i]);
  }


  String estado = "respuesta=" + arcana.respuesta.substring(1, 2);

  arcana.estado(estado);
  ///////////////////////////////
  delay(20);
}

void reset() {
  for (i = 0; i < 8; i ++) {
    digitalWrite(LED[i], LOW);
  }

  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
}
