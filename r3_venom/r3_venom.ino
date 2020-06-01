#include <Arcana3.h>
#include <Servo.h>

Arcana3 arcana("venom", true);

int counter = 0;
Servo myservo;  // create servo object to control a servo
long nextTime = 0;
int pos;

void setup() {
  pinMode(D2, OUTPUT); //alarma
  myservo.attach(D2);  // attaches the servo on GIO2 to the servo object

  arcana.init();
  delay(50);
  arcana.configurar();
  delay(50);
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50;

    if (arcana.status == 0) {
      myservo.write(90);              // tell servo to go to position in variable 'pos'
    }
    if (arcana.status == 1) {
      myservo.write(90);              // tell servo to go to position in variable 'pos'
    }
    if (arcana.status == 3) {
      myservo.write(90);              // tell servo to go to position in variable 'pos'
    }

  }
  counter--;

  if (arcana.status == 2) {
    if (arcana.acertijo(3)) {
      if (millis() > nextTime) {
        pos = random(-8, 9);
        myservo.write(90 + pos);
        int tiempo = abs(pos) * 500;
        nextTime = millis() + 4500 - tiempo;
      }
    } else {
      myservo.write(90);              // tell servo to go to position in variable 'pos'
    }
  }

  String estado = "";
  estado += "&Velocidad="+String(pos);
  arcana.estado(estado);


  ///////////////////////////////
  delay(20);
}
