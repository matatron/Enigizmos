#include <Arcana.h>

int counter = 0;
Arcana arcana("esfinge", true);

bool isReset = false;
bool cofre0 = false;
bool cofre1 = false;
bool cofre2 = false;
String estado = "off";

unsigned long badHumo;
unsigned long lastHumo;
int freqHumo = 5;

void setup() {
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  reset();

  Serial.begin(115200);
  arcana.init();
  delay(50);
  
  freqHumo = arcana.config.toInt();
}
void loop() {
  String cofres = "";
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
  if (arcana.respuesta.substring(1, 3).equals("on") && !isReset) {
    reset();
  }

  if (arcana.respuesta.toInt() == 16) {
    estado = "esperando";
    cofre0 = false;
    cofre1 = false;
    cofre2 = false;
  }
  if (arcana.respuesta.toInt() >= 17) {

    if (digitalRead(D5) == HIGH & !cofre0) {
      delay(300);
      if (digitalRead(D5) == HIGH & !cofre0) {
        cofre0 = true;
        badHumo = millis();
      }
    }

    if (digitalRead(D6) == HIGH & !cofre1) {
      delay(300);
      if (digitalRead(D6) == HIGH & !cofre1) {
        cofre1 = true;
      }
    }

    if (digitalRead(D7) == HIGH & !cofre2) {
      delay(300);
      if (digitalRead(D7) == HIGH & !cofre2) {
        cofre2 = true;
        badHumo = millis();
      }
    }

    if (cofre1 && estado == "esperando") {
      estado = "won";
      isReset = false;
      digitalWrite(D2, LOW);
      counter = 0;
    }

    if (cofre0 && estado == "esperando") {
      estado = "lost";
      isReset = false;
      digitalWrite(D2, LOW);
      counter = 0;
    }

  }

  if (arcana.respuesta.toInt() > 0) {
    if (arcana.respuesta.toInt() < 15) {
      if (millis() - lastHumo > freqHumo*1000) {
        digitalWrite(D1, LOW);
      }
      if (millis() - lastHumo > freqHumo*1000+5000) {
        lastHumo = millis();
        digitalWrite(D1, HIGH);
      }
    } else if (arcana.respuesta.toInt() < 18) {
      if (millis() - badHumo < 3000) {
        digitalWrite(D1, LOW);
      } else {
        digitalWrite(D1, HIGH);
      }
    }
  }


/*  
 *   
 cofres += "&cofres=";
  cofres += (digitalRead(D5) ? String("1") : String("0"));
  cofres += (digitalRead(D6) ? String("1") : String("0"));
  cofres += (digitalRead(D7) ? String("1") : String("0"));
*/  
  cofres += (cofre0 ? String("&cofre0=abierto") : String("&cofre0=cerrado"));
  cofres += (cofre1 ? String("&cofre1=abierto") : String("&cofre1=cerrado"));
  cofres += (cofre2 ? String("&cofre2=abierto") : String("&cofre2=cerrado"));

  arcana.estado("estado=" + estado + cofres);

  ///////////////////////////////
  delay(20);
}

void reset() {
  lastHumo = millis();
  badHumo = millis();
  cofre0 = false;
  cofre1 = false;
  cofre2 = false;
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  isReset = true;
  estado = "off";
}

