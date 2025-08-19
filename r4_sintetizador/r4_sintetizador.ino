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
#include <Adafruit_NeoPixel.h>

Arcana3 arcana("sintetizador", true);

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, D2, NEO_GRB + NEO_KHZ800);

int counter = 0;
bool isReset = false;
bool isPowerOn = false;
bool isCompleted = false;
uint8_t i;
 

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, INPUT_PULLUP);
  pinMode(D8, INPUT);
  Serial.begin(115200);
  while (!Serial);
  arcana.init();
  reset();
  delay(50);


}

void loop() {
  if (counter <= 0) {
    arcana.conectar();
    counter = 30;
  }
  if(arcana.acertijo((int) arcana.config["generador"])) {
    isPowerOn = true;
  }
  digitalWrite(D0, isPowerOn);
  if(arcana.status == 2 && !isCompleted && isPowerOn) {
    Serial.print ("check  ");
    Serial.print (digitalRead(D1));
    Serial.print (digitalRead(D2));
    Serial.print (digitalRead(D7));
    Serial.print ("  ");
    Serial.println(digitalRead(D8));
    if (isPowerOn && digitalRead(D8) == HIGH && !isCompleted) {
      digitalWrite(D5, LOW);
      delay(5000);
      digitalWrite(D5, HIGH);
      if (digitalRead(D1) && digitalRead(D2) && digitalRead(D7)) {
        isCompleted = true;
        counter = 30;
        delay(500);
        digitalWrite(D6, LOW);
        Serial.println("Activado manual!");
      }else{
        digitalWrite(D3, HIGH);
        delay(1000);
        digitalWrite(D3, LOW);
      }

    }
  }
  if(arcana.acertijo((int) arcana.config["acertijo"]) && !isCompleted) {
    isCompleted = true;
    counter = 30;
    digitalWrite(D6, LOW);
    Serial.println("Activado remoto!");
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
  }
  if (arcana.status == 3 && !isReset) {
    reset();
  }

  String estado = "activo=" + (isCompleted ? String("si") : String ("no"));
  estado += "&reset=" + (isReset ? String("si") : String ("no"));
  estado += "&status=" + (String) arcana.status;
  estado += "&energizado=" + (isPowerOn ? String("si") : String ("no"));
  estado += "&liquidos=" + (digitalRead(D1) ? String("1") : String("0")) + (digitalRead(D2) ? String("1") : String("0")) + (digitalRead(D7) ? String("1") : String("0"));

  arcana.estado(estado);


  ///////////////////////////////
  delay(50);
  counter --;
}

void reset() {
  Serial.println("Reset!");
  isReset = true;
  isCompleted = false;
  isPowerOn = false;
  digitalWrite(D3, LOW);
  digitalWrite(D0, LOW);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
}
