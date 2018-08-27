#include <Arcana.h>

int counter = 0;
Arcana arcana("apolo", true);

bool apoloWon = false;
bool zeusWon = false;
bool isReset = false;

void setup() {
  pinMode(D0, INPUT);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  reset();

  Serial.begin(115200);
  arcana.init();
  delay(50);
}
void loop() {
  String estado = "";
  if (counter == 0) {
    arcana.conectar();
    counter = 50;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.toInt() == 0 && !isReset) {
    reset();
  }
  if (arcana.respuesta.toInt() >= 1) {
    isReset = false;
  }

  if (!apoloWon) {
    if (digitalRead(D0) == LOW && digitalRead(D1) == LOW && digitalRead(D2) == LOW && digitalRead(D3) == LOW) {
      apoloWon = true;
    }
  }

  if (!zeusWon) {
    if (digitalRead(D5) == LOW) {
      zeusWon = true;
      digitalWrite(D6, HIGH);
      digitalWrite(D7, HIGH);
    }

  }
 
  estado += (zeusWon ? String("zeus=on") : String("zeus=off"));
  estado += String("&");
  estado += (apoloWon ? String("apolo=on") : String("apolo=off"));
  estado += String("&cuerdas=");
  estado += String(1-digitalRead(D0));
  estado += String(1-digitalRead(D1));
  estado += String(1-digitalRead(D2));
  estado += String(1-digitalRead(D3));

  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  apoloWon = false;
  zeusWon = false;
  isReset = true;
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
}
