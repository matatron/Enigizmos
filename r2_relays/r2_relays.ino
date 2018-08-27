#include <Arcana.h>
#include <Servo.h>

Arcana arcana("relaysPared", false, true);

int counter = 0;
bool isReset = false;
bool fxAnabel = false;
bool fxOlivia = false;
Servo servo1;


void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT); // pizarra
  pinMode(D2, OUTPUT); // solenoide olivia
  pinMode(D3, OUTPUT); // candelas
  pinMode(D5, OUTPUT); // ventilador
  pinMode(D6, OUTPUT); // luz final
  servo1.attach(D7);

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

  if (arcana.respuesta.charAt(3) == '1') {
    digitalWrite(D1, LOW);
    isReset = false;
  }

  if (arcana.respuesta.charAt(21) == '1') {
    digitalWrite(D3, LOW);
    isReset = false;
  }

  if (arcana.respuesta.charAt(22) == '1') {
    isReset = false;
    if (!fxOlivia) {
      HarlemShake();
      fxOlivia = true;
    }
  } else {
    fxOlivia = false;
  }

//  if (arcana.respuesta.charAt(23) == '1') {
//    digitalWrite(D5, LOW);
//    isReset = false;
//  }
  if (arcana.respuesta.charAt(24) == '1') {
//    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);
    isReset = false;
  }

  if (arcana.respuesta.charAt(5) == '1' && !fxAnabel) {
    delay(1000);
    servo1.write(100);
    delay(1000);
    servo1.write(75);
    fxAnabel = true;
    isReset = false;
  }
  if (arcana.respuesta.charAt(5) == '0' && fxAnabel) {
    fxAnabel = false;
    isReset = false;
  }


  String estado = "Anabelle=" + (fxAnabel ? String("presente") : String("escondida"));
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  servo1.write(75);
  fxAnabel = false;
  fxOlivia = false;
  isReset = true;
}

void HarlemShake() {
  int n = random(10, 15);
  while (n > 0) {
    n--;
    digitalWrite(D2, LOW);
    delay(random(100, 400));
    digitalWrite(D2, HIGH);
    delay(random(100, 400));
  }
}
