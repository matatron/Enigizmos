#include <Arcana.h>
#include <Servo.h>

Arcana arcana("servoluz", true, true);

int counter = 0;
bool isReset = false;
bool isDone = false;
int movidas = 0;
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


  if (arcana.respuesta.charAt(7) == '1' && !isDone) {
    servo1.write(175);
    delay(500);
    servo1.write(5);
    delay(1000);
    servo1.write(175);
    delay(500);
    servo1.write(5);
    delay(1000);
    servo1.write(175);
    delay(500);
    servo1.write(5);
    delay(3000);
    servo1.write(175);
    delay(500);
    servo1.write(5);
    delay(1000);
    servo1.write(175);
    delay(500);
    servo1.write(5);
    isDone = true;
    isReset = false;
    movidas ++;
  }
  if (arcana.respuesta.charAt(7) == '0' && isDone) {
    isDone = false;
  }


  String estado = "Movidas="+String(movidas);
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  servo1.write(5);
  isReset = true;
  isDone = false;
  movidas = 0;
}


