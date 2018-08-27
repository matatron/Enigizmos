#include <Arcana.h>
#include <Servo.h>

Arcana arcana("maquinaHumo", false, true);

int counter = 0;
bool isReset = false;
bool isFXEnd = false;
unsigned long lastHumo;

void setup() {
  pinMode(D1, OUTPUT); // humo
  pinMode(D2, OUTPUT); // ventilador

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

  if (arcana.respuesta.charAt(21) == '0') {
    if (millis() - lastHumo > 60000) {
      digitalWrite(D1, LOW);
    }
    if (millis() - lastHumo > 61000) {
      lastHumo = millis();
      digitalWrite(D1, HIGH);
    }
  }
  if (arcana.respuesta.charAt(23) == '1') {
    digitalWrite(D2, LOW);
    isReset = false;
  }else{
    digitalWrite(D2, HIGH);
  }
  
  if (arcana.respuesta.charAt(24) == '1') {
    digitalWrite(D2, HIGH);
    isReset = false;
    if (!isFXEnd) {
      digitalWrite(D1, LOW);
      isFXEnd = true;
    }
  }



  String estado = "HumoEn=";
  if (arcana.respuesta.charAt(21) == '0') {
    estado += String(floor(60 - (millis() - lastHumo) / 1000));
  } else {
    estado += "off";
  }
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  lastHumo = millis();
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  isFXEnd = false;
  isReset = true;
}
