#include <Arcana.h>
#include <Servo.h>

Arcana arcana("tormenta", true);


void setup() {
  reset();

  arcana.init();
  delay(50);
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 100 ;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.substring(1, 4).equals("off") && !isReset) {
    reset();
  }
  if (arcana.respuesta.toInt() == 0 && !isReset) {
    reset();
  }

  if (arcana.respuesta.toInt() >= 7) {
  }

  String estado = "intensidad=" + String(intensidad);
//  estado += "&hefesto=" + (r1Open ? String("abierta") : String("cerrada"));
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
}
