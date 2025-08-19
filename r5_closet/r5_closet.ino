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

Arcana3 arcana("closet", true);

int counter = 0;
bool isReset = false;
bool isCompleted = false;
int i;
 

void setup() {
  pinMode(D1, OUTPUT);    // LED pin as output.
  pinMode(D2, OUTPUT);    // LED pin as output.
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
  }
  if(arcana.acertijo((int) arcana.config["acertijo"]) && !isCompleted) {
    isCompleted = true;
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    delay(5000);
    digitalWrite(D2, HIGH);
  }

  digitalWrite(D2, !arcana.acertijo((int) arcana.config["humo"]));
  if (arcana.status == 0) {
    reset();
  }
  if (arcana.status == 1 && !isReset) {
    reset();
  }
  if (arcana.status == 2 && isReset) {
    //arcana.configurar();
    isReset = false;
  }
  if (arcana.status == 3 && !isReset) {
    reset();
  }


  String estado = "reset=" + (isReset ? String("si") : String ("no"));
  estado += "&abierto=" + (isCompleted ? String("si") : String ("no"));
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
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
}
