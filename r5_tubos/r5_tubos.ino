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

Arcana3 arcana("tubos", true);

int counter = 0;
bool isReset = false;
bool isInitialized = false;
bool isCompleted = false;
uint8_t i;
String idValues = "0000000";
String correctValues = "0000000";
uint8_t correctos;
 

void setup() {
  Serial.begin(115200);
  while (!Serial);
  reset();
  arcana.init();
  delay(50);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
}

void loop() {
  bool missingOne = false;

  if(arcana.status == 2 && !isCompleted) {
    for(i=0; i<7; i++) {
      int value = 0;
      digitalWrite(D1, bitRead(i,0));
      digitalWrite(D2, bitRead(i,1));
      digitalWrite(D3, bitRead(i,2));
      delay(10);
      if (digitalRead(D5) == LOW) value += 1;
      if (digitalRead(D6) == LOW) value += 2;
      if (digitalRead(D7) == LOW) value += 4;
      if (value == 0) missingOne = true;
      idValues[i] = String(value)[0];
    }
    if (!missingOne && !isInitialized) {
      delay(500);
      for(i=0; i<7; i++) {
        int value = 0;
        digitalWrite(D1, bitRead(i,0));
        digitalWrite(D2, bitRead(i,1));
        digitalWrite(D3, bitRead(i,2));
        delay(10);
        if (digitalRead(D5) == LOW) value += 1;
        if (digitalRead(D6) == LOW) value += 2;
        if (digitalRead(D7) == LOW) value += 4;
        if (value == 0) missingOne = true;
        idValues[i] = String(value)[0];
      }

      bool errorRepeat = false;
      for(i=0; i<6; i++) {
        for(int j=i+1; j<7; j++) {
          if (idValues[i] == idValues[j]) {
            Serial.println("ERROR: Valor Repetido");
            errorRepeat = true;
          }
        }
      }
      if (errorRepeat) idValues = "1234567";

      // set final values
      
      switch ((int)arcana.config["people"]) {
        case 2:
          correctValues[5] = idValues[0];
          correctValues[1] = idValues[1];
          correctValues[2] = idValues[2];
          correctValues[6] = idValues[3];
          correctValues[4] = idValues[4];
          correctValues[3] = idValues[5];
          correctValues[0] = idValues[6];
        break;
        case 3:
          correctValues[5] = idValues[0];
          correctValues[3] = idValues[1];
          correctValues[2] = idValues[2];
          correctValues[6] = idValues[3];
          correctValues[4] = idValues[4];
          correctValues[1] = idValues[5];
          correctValues[0] = idValues[6];
        break;
        default:
          correctValues[5] = idValues[0];
          correctValues[3] = idValues[1];
          correctValues[2] = idValues[2];
          correctValues[6] = idValues[3];
          correctValues[1] = idValues[4];
          correctValues[4] = idValues[5];
          correctValues[0] = idValues[6];
        break;
      }
      Serial.println("Primera vez: "+idValues);
      Serial.println("Solucion: "+correctValues);
      isInitialized = true;
    } 

    if(arcana.acertijo((int) arcana.config["acertijo"])) {
      isCompleted = true;
      counter = 30;
      Serial.println("Activado remoto!");
    }
    if (idValues == correctValues) {
      isCompleted = true;
      counter = 0;
      Serial.println("Activado manual!");
    }
    correctos = 0;
    for(i=0; i<7; i++) {
      if (idValues[i] == correctValues[i]) {
        correctos++;
      }
    }

  }
  
  if (arcana.status == 0 && !isReset) {
    reset();
  }
  if (arcana.status == 1 && !isReset) {
    reset();
  }
  if (arcana.status == 2 && isReset) {
    arcana.configurar();
    isReset = false;
  }
  if (arcana.status == 3 && !isReset) {
    reset();
  }

  String estado = "activo=" + (isCompleted ? String("si") : String ("no"));
  estado += "&reset=" + (isReset ? String("si") : String ("no"));
  estado += "&actuales=" + String(idValues);
  estado += "&solucion=" + String(correctValues);
  estado += "&llenos=" + (!missingOne ? String("si") : String ("no"));
  estado += "&correctos=" + String(correctos);
  estado += "&status=" + (String) arcana.status;
  arcana.estado(estado);


  ///////////////////////////////
  delay(50);
  counter --;

  if (counter <= 0) {
    arcana.conectar();
    counter = 30;
  }

}

void reset() {
  isReset = true;
  isInitialized = false;
  isCompleted = false;
  idValues = "0000000";
  correctValues = "8888888";
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
}
