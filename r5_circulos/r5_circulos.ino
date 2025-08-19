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

Arcana3 arcana("circulos", true);


int counter = 0;
bool isReset = false;
bool isCompleted = false;
uint8_t i;
unsigned long lastBeep = 0;

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, OUTPUT); //buzz
  pinMode(D5, OUTPUT); // luz polar
  pinMode(D6, OUTPUT); // relay2
  pinMode(D7, INPUT_PULLUP);
  Serial.begin(115200);
  while (!Serial);
  arcana.init();
  reset();
  delay(50);
  Serial.print ("SETUP Complete");


}

void loop() {
  if (counter <= 0) {
    arcana.conectar();
    counter = 30;
  }
  if(arcana.status == 2 && !isCompleted) {
    Serial.print ("check  ");
    Serial.print (digitalRead(D1));
    Serial.print (digitalRead(D2));
    Serial.print (digitalRead(D7));
    Serial.print ("  ");

    if (digitalRead(D1)==HIGH && digitalRead(D2)==HIGH && digitalRead(D7)==HIGH) {
      digitalWrite(D3, HIGH);
      delay(400);
      digitalWrite(D3, LOW);
      digitalWrite(D5, LOW);
      isCompleted = true;
      counter = 0;
      Serial.println("Activado manual!");
    }    
  }
  if(arcana.acertijo((int) arcana.config["acertijo"]) && !isCompleted) {
    isCompleted = true;
    counter = 30;
    digitalWrite(D3, HIGH);
    delay(400);
    digitalWrite(D3, LOW);
    digitalWrite(D5, LOW);
    Serial.println("Activado remoto!");
  }
  if(arcana.acertijo((int) arcana.config["relay2"])) {
    digitalWrite(D6, LOW);
    Serial.println("Activado remoto!");
  }
  
  if (arcana.status == 0 || arcana.status == 3) {
    if (digitalRead(D1)==HIGH || digitalRead(D2)==HIGH || digitalRead(D7)==HIGH) {
      if (millis() - lastBeep > 30000) {
        Serial.println("Beep!");
        lastBeep = millis();
        digitalWrite(D3, HIGH);
        delay(100);
        digitalWrite(D3, LOW);
        delay(100);
        digitalWrite(D3, HIGH);
        delay(100);
        digitalWrite(D3, LOW);
        delay(100);
        digitalWrite(D3, HIGH);
        delay(100);
        digitalWrite(D3, LOW);
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
    //arcana.configurar();
    isReset = false;
  }
  if (arcana.status == 3 && !isReset) {
    reset();
  }

  String estado = "activo=" + (isCompleted ? String("si") : String ("no"));
  estado += "&reset=" + (isReset ? String("si") : String ("no"));
  estado += "&status=" + (String) arcana.status;
  estado += "&circulos=" + (digitalRead(D2) ? String("1") : String("0")) + (digitalRead(D1) ? String("1") : String("0")) + (digitalRead(D7) ? String("1") : String("0"));

  arcana.estado(estado);


  ///////////////////////////////
  delay(50);
  counter --;
}

void reset() {
  Serial.println("Reset!");
  isCompleted = false;
  isReset = true;
  lastBeep = 0;
  digitalWrite(D3, LOW);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
}
