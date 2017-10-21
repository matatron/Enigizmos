#include <Arcana.h>
//#include <Servo.h>

//Servo myservo;
int counter = 0;
int contadorHumo = 0;
Arcana arcana("esfinge", true);

bool isReset = false;
bool cofre0 = false;
bool cofre1 = false;
bool cofre2 = false;
bool flagHumo = false;
unsigned long badHumo;
unsigned long lastHumo;

void setup() {
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, OUTPUT);
  reset();

  Serial.begin(115200);
  arcana.init();
  delay(50);
}
void loop() {
  String cofres = "";
  String estado = "off";
  if (counter == 0) {
    arcana.conectar();
    counter = 50;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.toInt() == 0 && !isReset) {
    reset();
  }
  if (arcana.respuesta.substring(1, 4).equals("off") && !isReset) {
    reset();
  }

  Serial.print(digitalRead(D1));
  Serial.print(digitalRead(D2));
  Serial.println(digitalRead(D5));


  if (digitalRead(D1) == LOW & !cofre0) {
    cofre0 = true;
    badHumo = millis();
  }

  if (digitalRead(D2) == LOW & !cofre1) {
    cofre1 = true;
  }

  if (digitalRead(D5) == LOW & !cofre2) {
    cofre2 = true;
    badHumo = millis();
  }

  if (cofre1) {
    estado = "won";
    isReset = false;
  }


  if (arcana.respuesta.toInt() < 12) {
    if (millis() - lastHumo > 120000 && flagHumo == false && contadorHumo <= 20 && contadorHumo > 5) {
      contadorHumo++;
      flagHumo = true;
      digitalWrite(D6, LOW);
    }
    if (millis() - lastHumo > 130000 && flagHumo == true) {
      flagHumo = false;
      lastHumo = millis();
      digitalWrite(D6, HIGH);
    }
  } else {
    if (millis() - badHumo < 3000) {
      digitalWrite(D6, LOW);
    } else {
      digitalWrite(D6, HIGH);
    }
  }


  cofres += (cofre0 ? String("&cofre0=abierto") : String("&cofre0=cerrado"));
  cofres += (cofre1 ? String("&cofre1=abierto") : String("&cofre1=cerrado"));
  cofres += (cofre2 ? String("&cofre2=abierto") : String("&cofre2=cerrado"));
  arcana.estado("estado=" + estado + cofres);

  ///////////////////////////////
  delay(20);
}

void reset() {
  contadorHumo = 0;
  flagHumo = false;
  lastHumo = millis();
  badHumo = millis();
  digitalWrite(D6, HIGH);
  isReset = true;
}

