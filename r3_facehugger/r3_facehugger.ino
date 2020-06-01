#include <Arcana3.h>
#include <Servo.h>

Arcana3 arcana("facehugger", true);

int counter = 0;
bool isReset = false;
bool isDoorOpen = false;
Servo servo1;
Servo servo2;

unsigned long fhTime = 0;

void setup() {
  pinMode(D1, INPUT_PULLUP); //botones
  pinMode(D2, INPUT_PULLUP); //botones
  servo1.attach(D5);
  servo2.attach(D6);
  Serial.begin(115200);

  reset();

  arcana.init();
  delay(50);
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50;

    if (arcana.status == 0 && !isReset) {
      arcana.configurar();
      reset();
    }
    if (arcana.status == 1 && !isReset) {
      reset();
    }
  }
  counter--;

  if (arcana.status == 2) {
    isReset = false;
    if (digitalRead(D1) == LOW && digitalRead(D2) == LOW) {
      isDoorOpen = true;
      arcana.conectar();
    }

    if (arcana.acertijo(5)) {
      if (millis() > fhTime) {
        fhTime = millis() + 200;
        int angulo = random(0, 90);
        servo1.write(20 + angulo);
        servo2.write(160 - angulo);
      }
    } else {
      servo1.write(20);
      servo2.write(160);
    }
  }

  String estado = "";
  estado += "&Puerta0=" + (isDoorOpen ? String("abierta") : String("cerrada"));
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  servo1.write(20);
  servo2.write(160);
  isReset = true;
  isDoorOpen = false;
}
