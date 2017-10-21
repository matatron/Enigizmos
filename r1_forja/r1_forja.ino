#include <Arcana.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
int counter = 0;
Arcana arcana("forja", true);

bool forjaOn = false;
bool baculoOn = false;
bool isReset = false;
bool r1Open = false;
bool r2Open = false;
bool r3Open = false;

int intensidad = 0;

void setup() {
  pinMode(D0, OUTPUT); //led intesidad
  pinMode(D1, INPUT); //sensor laser

  pinMode(D2, OUTPUT); //LED hermes
  pinMode(D3, INPUT_PULLUP); //boton hermes


  servo1.attach(D5);
  servo2.attach(D6);
  servo3.attach(D7);
  reset();

  Serial.begin(115200);
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
  if (arcana.respuesta.toInt() < 4 && !isReset) {
    reset();
  }

  if (!forjaOn) {
    if (intensidad > 1000) {
      isReset = false;
      forjaOn = true;
      analogWrite(D0, 0); //intesidad

      servo1.write(175);
      delay(2000);
      servo1.write(5);
      r1Open = true;

    } else {
      if (digitalRead(D1) == HIGH) {
        intensidad += 6;
      } else {
        intensidad -= 2;
        if (intensidad < 0) intensidad = 0;
      }
      analogWrite(D0, 1023 - intensidad); //intesidad
    }
  }
  if (arcana.respuesta.toInt() >= 4 && r1Open == false) {
    isReset = false;
    forjaOn = true;
    analogWrite(D0, 0); //intesidad
    servo1.write(175);
    delay(2000);
    servo1.write(5);
    r1Open = true;
  }


  if (digitalRead(D3) == LOW && r2Open == false ) {
    baculoOn = true;
    r2Open = true;

    digitalWrite(D2, LOW);
    servo2.write(175);
    delay(2000);
    servo2.write(5);
  }
  if (arcana.respuesta.toInt() >= 7 && r2Open == false) {
    baculoOn = true;
    r2Open = true;

    digitalWrite(D2, LOW);
    servo2.write(175);
    delay(2000);
    servo2.write(5);
  }

  if (arcana.respuesta.toInt() >= 8 && r3Open == false) {
    servo3.write(175);
    delay(2000);
    servo3.write(5);
    r3Open = true;
  }

  String estado = "intensidad=" + String(intensidad);
  estado += "&hefesto=" + (r1Open ? String("abierta") : String("cerrada"));
  estado += "&hermes=" + (r2Open ? String("abierta") : String("cerrada"));
  estado += "&artemisa=" + (r3Open ? String("abierta") : String("cerrada"));
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  isReset = true;
  forjaOn = false;
  baculoOn = false;
  r1Open = false;
  r2Open = false;
  r3Open = false;
  analogWrite(D0, 1023); //intesidad
  digitalWrite(D2, HIGH); //LED hermes
  servo1.write(5);
  servo2.write(5);
  servo3.write(5);
}
