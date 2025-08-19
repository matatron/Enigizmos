#define LED_BUILTIN 2
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define TX 1
#define RX 3
#define SD2 9
#define SD3 10
#include <Arcana3.h>

Arcana3 arcana("salida", false);

int counter = 0;
bool isReset = false;
bool isStarted = false;
bool isCompleted = false;
int i;
 

void setup() {
  pinMode(D1, OUTPUT);    // LED pin as output.
  pinMode(D2, OUTPUT);    // LED pin as output.
  pinMode(D3, INPUT_PULLUP);    // LED pin as output.
  digitalWrite(D2, LOW);
  Serial.begin(115200);
  while (!Serial);
  reset();
  arcana.init();
  delay(50);
}

void loop() {
  if (counter <= 0) {
    arcana.conectar();
    counter = 25;
    Serial.println(digitalRead(D3));
  }
  if(arcana.acertijo((int) arcana.config["acertijo"]) && !isCompleted) {
    isCompleted = true;
    digitalWrite(D1, HIGH);
  }
  if(arcana.acertijo(((int) arcana.config["acertijo"])-1)) {
    digitalWrite(D1, HIGH);
  }
  if(arcana.acertijo(((int) arcana.config["acertijo"])-1) && digitalRead(D3) == HIGH && !isCompleted) {
    isCompleted = true;
  }

  if (arcana.status == 0) {
    reset();
  }
  if (arcana.status == 1 && !isReset) {
    reset();
  }
  if (arcana.status == 2 && isReset) {
    //arcana.configurar();
    isReset = false;
    isStarted = true;
  }
  if (arcana.status == 2 && !isStarted) {
    isStarted = true;
  }
  if (arcana.status == 3 && !isReset) {
    reset();
  }

  if (arcana.status == 3 && !isStarted && digitalRead(D3) == LOW) {
    isStarted = true;
  }


  String estado = "reset=" + (isReset ? String("si") : String ("no"));
  estado += "&escape=" + (isCompleted ? String("completo") : String ("pendiente"));
  estado += "&inicio=" + (isStarted ? String("completo") : String ("pendiente"));
  estado += "&status=" + (String) arcana.status;
  //Serial.println(estado);
  arcana.estado(estado);

  ///////////////////////////////
  counter --;
  delay(25);
}

void reset() {
  isReset = true;
  isCompleted =false;
  isStarted = false;
  digitalWrite(D1, LOW);
}
