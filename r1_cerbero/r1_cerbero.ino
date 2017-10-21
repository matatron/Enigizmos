#include <Arcana.h>
//#include <Servo.h>

//Servo myservo;
int counter = 0;
Arcana arcana("cerbero", true);

bool isWon = false;
bool isReset = false;

void setup() {
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
//  myservo.attach(D0);
  reset();

  Serial.begin(115200);
  arcana.init();
  delay(50);
}
void loop() {
  String simbolos = "";
  String estado = "off";
  if (counter == 0) {
    arcana.conectar();
    counter = 50;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.toInt() == 0 && !isReset) {
    isReset = true;
    reset();
  }

  isWon = true;
  
  if(digitalRead(D0)) {
    simbolos += "1";
  } else {
    isWon = false;
  }
  if(digitalRead(D1)) {
    simbolos += "2";
  } else {
    isWon = false;
  }
  if(digitalRead(D2)) {
    simbolos += "3";
  } else {
    isWon = false;
  }

  if(isWon) {
    estado = "on";
    isReset = false;
  }

  arcana.estado("simbolos="+simbolos+"&estado="+estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  isWon = false;
}
