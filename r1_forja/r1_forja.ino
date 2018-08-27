
#include <Arcana.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
int counter = 0;
Arcana arcana("forja", true, true);


bool forjaOn = false;
bool baculoOn = false;
bool isReset = false;
bool r1Open = false;
bool r2Open = false;
bool r3Open = false;

int intensidad = 0;

void setup() {

  pinMode(D0, OUTPUT); //LED hefesto
  pinMode(D1, INPUT); //sensor laser
  // D2 neopixels
  pinMode(D2, INPUT_PULLUP); //boton hermes
  pinMode(D3, OUTPUT); //LED hermes


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
    counter = 50 ;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.substring(1, 4).equals("off") && !isReset) {
    reset();
  }
  if (arcana.respuesta.substring(1, 4).equals("on") && !isReset) {
    reset();
  }
  //  if (arcana.respuesta.toInt() == 0 && !isReset) {
  //    reset();
  //  }

  if (arcana.respuesta.charAt(4) == '1') {
    if (!r1Open) {
      isReset = false;
      forjaOn = true;
      intensidad = 602;
      analogWrite(D0, LOW);
      servo1.write(175);
      delay(1000);
      servo1.write(5);
      r1Open = true;
    }
  } else {
    if (r1Open) {
      forjaOn = false;
      intensidad = 0;
      analogWrite(D0, HIGH);
      r1Open = false;
    }
  }
  if (!forjaOn) {
    if (intensidad > 600) {
      forjaOn = true;
      isReset = false;

      analogWrite(D0, LOW);
      servo1.write(175);
      delay(1000);
      servo1.write(5);
      r1Open = true;
      counter = 0;
    } else {
      if (arcana.respuesta.charAt(3) == '1') {
        if (digitalRead(D1) == HIGH) {
          intensidad += 6;
        } else {
          intensidad -= 3;
          if (intensidad < 0) intensidad = 0;
        }
        analogWrite(D0, 1024 - intensidad * 1.7);
      }
    }
  }


  if (arcana.respuesta.charAt(7) == '1') {
    if (!r2Open) {
      isReset = false;
      baculoOn = true;
      r2Open = true;

      digitalWrite(D3, LOW);
      servo2.write(175);
      delay(1000);
      servo2.write(5);
    }
  } else {
    if (r2Open) {
      r2Open = false;
    }
  }
  if (digitalRead(D2) == LOW && !baculoOn) {
    baculoOn = true;
    r2Open = true;

    digitalWrite(D3, LOW);
    servo2.write(175);
    delay(1000);
    servo2.write(5);
    counter = 0;
  }

  if (arcana.respuesta.charAt(8) == '1') {
    if (!r3Open) {
      isReset = false;
      servo3.write(175);
      delay(1000);
      servo3.write(5);
      r3Open = true;
    }
  } else {
    if (r3Open) {
      r3Open = false;
    }
  }

  String estado = "intensidad=" + String(intensidad);
  estado += "&1_Hefesto=" + (r1Open ? String("abierta") : String("cerrada"));
  estado += "&2_Hermes=" + (r2Open ? String("abierta") : String("cerrada"));
  estado += "&3_Artemisa=" + (r3Open ? String("abierta") : String("cerrada"));
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
  //  for (int i = 0; i < 8; i++) {
  //  }
  //  delay(100);
  intensidad = 0;
  analogWrite(D0, 1024);
  digitalWrite(D3, HIGH); //LED hermes
  servo1.write(5);
  servo2.write(5);
  servo3.write(5);
}
