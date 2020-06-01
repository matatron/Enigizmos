#include <Arcana3.h>
#include <Servo.h>

Arcana3 arcana("laser", true);

int counter = 0;
Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
int posX = 100;
int posY = 90;
bool isReset = false;
bool enPosicion = false;
long lastTime = 0;
long laserTime = 0;
int puzzles[10] = {0, 0, 90, 90, 0, 0, 0, 0, 0, 0};

void setup() {
  myservo1.attach(D0);  // attaches the servo on GIO2 to the servo object
  myservo2.attach(D6);  // attaches the servo on GIO2 to the servo object
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  arcana.init();
  delay(50);
  configurar();
  delay(50);
  reset();
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50;

    if (arcana.status == 0 && !isReset) {
      configurar();
      reset();
    }
    if (arcana.status == 1) {
    }
    if (arcana.status == 3 && !isReset) {
      reset();
    }

  }
  counter--;

  if (arcana.status == 2) {
    if (digitalRead(D1) == LOW && millis()>lastTime) {
      lastTime = millis()+150;
      laserTime = millis()+3000;
      posX = max(posX - 2, 40);
    }
    if (digitalRead(D2) == LOW && millis()>lastTime) {
      lastTime = millis()+150;
      laserTime = millis()+3000;
      posX = min(posX + 2, 160);
    }
    if (digitalRead(D3) == LOW && millis()>lastTime) {
      lastTime = millis()+150;
      laserTime = millis()+3000;
      posY = max(posY - 2, 30);
    }
    if (digitalRead(D5) == LOW && millis()>lastTime) {
      lastTime = millis()+150;
      laserTime = millis()+3000;
      posY = min(posY + 2, 140);
    }
    if (abs(posX - puzzles[0]) < 5 && abs(posY - puzzles[1]) < 5 && millis() > laserTime) {
      enPosicion = true;
    }
  }


  myservo1.write(posX);
  myservo2.write(posY);

  String estado = "";
  estado += "&Laser="+(enPosicion? String("listo") : String("no"));
  estado += "&PosX="+String(posX);
  estado += "&PosY="+String(posY);
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  posX = puzzles[2];
  posY = puzzles[3];
  myservo1.write(posX);
  myservo2.write(posY);
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
