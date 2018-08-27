#include <Arcana.h>

Arcana arcana("piano", false);

int counter = 0;
bool isReset = false;
bool isDone = false;


void setup() {
  pinMode(D2, INPUT_PULLUP);
  reset();

  arcana.init();
  delay(50);
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50 ;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.substring(1, 4).equals("off") && !isReset) {
    reset();
  }
  if (arcana.respuesta.substring(1, 3).equals("on") && !isReset) {
    reset();
  }
  if (arcana.respuesta.toInt() == 0 && !isReset) {
    reset();
  }

  if (arcana.respuesta.toInt() > 0  && digitalRead(D2) == LOW) {
    isDone = true;
    isReset = false;
  }


  String estado = "piano=" + (isDone ? String("done") : String("waiting"));
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  isDone = false;
  isReset = true;
}

