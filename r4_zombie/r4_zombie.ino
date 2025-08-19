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

Arcana3 arcana("zombie", false);

int counter = 0;
bool isReset = false;
bool isAlive = false;
bool isScanning = false;
bool isPowerOn = false;
bool isCable1 = true;
bool isCable2 = true;
bool isCable3 = true;
uint8_t i;
unsigned long inicio;
unsigned long actual;

void setup() {
  pinMode(D0, OUTPUT);  //relay
  pinMode(D4, OUTPUT);        // LED pin as output.  ojos
  pinMode(D1, OUTPUT);        // rojo
  pinMode(D2, OUTPUT);        // blanco
  pinMode(D3, OUTPUT);        // negro -> azul
  pinMode(D5, INPUT_PULLUP);  // rojo
  pinMode(D6, INPUT_PULLUP);  // blanco
  pinMode(D7, INPUT_PULLUP);  // negro -> azul
  Serial.begin(115200);
  while (!Serial)
    ;
  reset();
  arcana.init();
  delay(50);
  Serial.println("Zombie Vivo en :");
  Serial.println((String)arcana.config["zombie"]);
  Serial.println("Scanner Activo en :");
  Serial.println((String)arcana.config["scanner"]);
}

void loop() {
  
  if (counter <= 0) {
    arcana.conectar();
    counter = 20;
  }
  if (arcana.status == 2) {

    if (arcana.acertijo((int)arcana.config["zombie"]) && !isAlive) {
      isAlive = true;
      inicio = millis();
      digitalWrite(D4, LOW);
    }

    if (isAlive) {
      actual = millis() - inicio;
      if (actual < 1000) {
        digitalWrite(D0, HIGH);
      } else {
        digitalWrite(D0, LOW);
      }
    }
    
    isPowerOn = (arcana.acertijo((int)arcana.config["generador"]));
  

    if (!isScanning && isPowerOn) {
      bool correct = true;
      //////
      digitalWrite(D1, LOW);
      delay(50);
      isCable1 = (digitalRead(D5) == HIGH); 
      if (isCable1) correct = false;
      Serial.print(digitalRead(D5));
      digitalWrite(D1, HIGH);
      delay(50);
      //////
      digitalWrite(D2, LOW);
      delay(50);
      isCable2 = (digitalRead(D6) == HIGH); 
      if (isCable2) correct = false;
      Serial.print(digitalRead(D6));
      digitalWrite(D2, HIGH);
      delay(50);
      //////
      digitalWrite(D3, LOW);
      delay(50);
      isCable3 = (digitalRead(D7) == HIGH); 
      if (isCable3) correct = false;
      Serial.println(digitalRead(D7));
      digitalWrite(D3, HIGH);

      if (correct) {
        isScanning = true;
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
    isReset = false;
    arcana.configurar();
  }
  if (arcana.status == 3 && !isReset) {
    reset();
  }



  String estado = "zombie=" + (isAlive ? String("vivo") : String("muerto"));
  estado += "&scannerListo=" + (isScanning ? String("si") : String("no"));
  estado += "&energizado=" + (isPowerOn ? String("si") : String("no"));
  estado += "&reset=" + (isReset ? String("si") : String("no"));
  estado += "&status=" + (String)arcana.status;
  estado += "&cables=" + (isCable1 ? String("0") : String("1")) + (isCable2 ? String("0") : String("1")) + (isCable3 ? String("0") : String("1"));
  arcana.estado(estado);


  Serial.println(estado);  
  ///////////////////////////////
  delay(50);
  counter--;
}

void reset() {
  Serial.println("Resetting v1");
  inicio = millis();
  isReset = true;
  isAlive = false;
  isScanning = false;
  isPowerOn = false;
 isCable1 = true;
 isCable2 = true;
 isCable3 = true;
  digitalWrite(D0, LOW);
  digitalWrite(D4, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
}
