#include <Arcana.h>
#include <Servo.h>

Arcana arcana("repisa", true, true);

int counter = 0;
bool isReset = false;
bool isDone = false;
Servo servo1;


void setup() {
  servo1.attach(D1);
  Serial.begin(115200);

  reset();

  arcana.init();
  delay(50);
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.substring(1, 4).equals("off") && !isReset) {
    reset();
  }
  if (arcana.respuesta.substring(1, 3).equals("on") && !isReset) {
    reset();
  }


  if (arcana.respuesta.charAt(12) == '1' && !isDone) {
    servo1.write(95);
    delay(2000);
    servo1.write(5);
    isDone = true;
    isReset = false;
  }
  if (arcana.respuesta.charAt(12) == '0' && isDone) {
    isDone = false;
  }


  String estado = "";
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  servo1.write(5);
  isReset = true;
  isDone = false;
}


