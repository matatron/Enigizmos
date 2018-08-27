#include <Arcana.h>
#include <Servo.h>

Servo myservo;
int counter = 0;
Arcana arcana("laberinto", true);

bool isPlaying = false;
bool isWon = false;
bool isReset = false;

void setup() {
  pinMode(D0, INPUT);
  pinMode(D6, OUTPUT);
  myservo.attach(D1);
  myservo.write(90);
  reset();

  Serial.begin(115200);
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
  if (arcana.respuesta.substring(1, 4).equals("on") && !isReset) {
    reset();
  }
  if (arcana.respuesta.toInt() == 0 && !isReset) {
    reset();
  }


  if (digitalRead(D0) == HIGH) {
    isWon = true;
    digitalWrite(D6, LOW);
    isReset = false;
  }
  if (arcana.respuesta.toInt() >= 13) {
    isWon = true;
    digitalWrite(D6, LOW);
    isReset = false;
  }

  arcana.estado(isWon ? String("laberinto=ganado") : String("laberinto=esperando"));
  ///////////////////////////////
  delay(20);
}

void reset() {
  isWon = false;
  isReset = true;
  digitalWrite(D6, HIGH);
  myservo.write(0);
  delay(500);
  myservo.write(90);
  delay(500);
}
