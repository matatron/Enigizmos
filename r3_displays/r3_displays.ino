#include <Arcana3.h>
#include "TM1637.h"

TM1637 display1(D1, D6);
TM1637 display2(D1, D5);
TM1637 display3(D1, D3);
TM1637 display4(D1, D2);

Arcana3 arcana("displays", true);

int counter = 0;
bool isReset = false;
int puzzles[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;

int m1 = 0;
int m2 = 0;
int m3 = 0;
int m4 = 0;

void setup() {
  pinMode(D0, OUTPUT); //botones
  pinMode(D1, OUTPUT); //botones
  pinMode(D2, OUTPUT); //botones
  pinMode(D3, OUTPUT); //botones
  pinMode(D4, OUTPUT); //botones
  pinMode(D5, OUTPUT); //botones
  pinMode(D6, OUTPUT); //botones
  pinMode(D7, OUTPUT); //botones

  display1.init();
  display1.set(4);// 0-7 to set brightness
  display2.init();
  display2.set(4);// 0-7 to set brightness
  display3.init();
  display3.set(4);// 0-7 to set brightness
  display4.init();
  display4.set(4);// 0-7 to set brightness

  reset();

  arcana.init();
  delay(50);
  configurar();
  delay(50);
  Serial.begin(115200);
}
void loop() {
  bool isMixed = false;
  if (counter == 0) {
    arcana.conectar();
    counter = 50;

    if (arcana.status == 0 && !isReset) {
      arcana.configurar();
      reset();
    }

    if (arcana.status == 3 && !isReset) {
      reset();
    }

  }
  counter--;

  if (arcana.status == 2) {
    isReset = false;

    c1 = arcana.doc["params"]["C1"]; //W
    c2 = arcana.doc["params"]["C2"]; //Z
    c3 = arcana.doc["params"]["C3"]; //Y
    c4 = arcana.doc["params"]["C4"]; //X

    m1 = c3 + c3 + c3;
    m2 = c2 + c1 + c1;
    m3 = c2 + c1 + c3;
    m4 = c4 + c1 + c3;

    isMixed = (m1 == puzzles[0] && m2 == puzzles[1] && m3 == puzzles[2] && m4 == puzzles[3]);

    if (arcana.acertijo(14)) {
      display1.display(3, m1 % 10);
      display1.display(2, (m1 / 10) % 10);
      display2.display(3, m2 % 10);
      display2.display(2, (m2 / 10) % 10);
      display3.display(3, m3 % 10);
      display3.display(2, (m3 / 10) % 10);
      display4.display(3, m4 % 10);
      display4.display(2, (m4 / 10) % 10);
    } else {
      display1.clearDisplay();
      display2.clearDisplay();
      display3.clearDisplay();
      display4.clearDisplay();
    }

  }


  String estado = "";
  estado += "&M1=" + String(m1);
  estado += "&M2=" + String(m2);
  estado += "&M3=" + String(m3);
  estado += "&M4=" + String(m4);
  estado += "&Combustible=" + (isMixed ? String("listo") : String("esperando") );
  arcana.estado(estado);


  ///////////////////////////////
  delay(20);
}

void reset() {
  display1.clearDisplay();
  display2.clearDisplay();
  display3.clearDisplay();
  display4.clearDisplay();

  isReset = true;
}

void configurar() {
  arcana.configurar();
  String myconfig = arcana.config;
  int i = 0;
  while (myconfig.indexOf(" ") > -1) {
    puzzles[i] = myconfig.substring(0, myconfig.indexOf(" ")).toInt();
    myconfig = myconfig.substring(myconfig.indexOf(" ") + 1);
    i++;
  }
  puzzles[i] = myconfig.toInt();
}
