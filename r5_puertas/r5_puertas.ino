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
#include <SPI.h>
#include <MFRC522.h>

Arcana3 arcana("puertas", true);

int counter = 0;
bool isReset = false;
bool isCompleted = false;
bool isWaiting = false;
uint8_t i;
constexpr uint8_t RST_PIN = D1;    // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D2;     // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

String serial = "";
String lastserial = "";
int ultimaTarjeta = 0;
bool alreadyBeep = false;

void setup() {
  pinMode(D0, OUTPUT);  // sala maquinas
  pinMode(D3, OUTPUT);  // puerta reactor
  pinMode(D8, OUTPUT);  // LED pin as output.
  Serial.begin(115200);
  while (!Serial);
  SPI.begin();
  reset();
  arcana.init();
  delay(50);
  Serial.println("Solucion:");
  Serial.println((String)arcana.config["solucion"]);
  Serial.println((String)arcana.config["emergencia"]);
  mfrc522.PCD_Init();                 // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  alreadyBeep = false;
  isWaiting = false;
}

void loop() {
  if (counter <= 0) {
    arcana.conectar();
    counter = 20;
  }
  digitalWrite(D0, arcana.acertijo((int)arcana.config["maquinas"]));
  /*if (!isCompleted) {
    if (arcana.acertijo((int)arcana.config["acertijo"])) {
      //Serial.println("Abierto!");
      isCompleted = true;
      digitalWrite(D3, HIGH);
      digitalWrite(D8, HIGH);
      delay(2000);
      digitalWrite(D8, LOW);
      digitalWrite(D3, LOW);
    }
  }else{
    if (!arcana.acertijo((int)arcana.config["acertijo"])) {
      isCompleted = false;
    }
  }*/

  if (mfrc522.PICC_IsNewCardPresent()) {
    serial = "";
    int sum = 0;
    if (mfrc522.PICC_ReadCardSerial()) {


      for (int x = 0; x < mfrc522.uid.size; x++) {
        // If it is less than 10, we add zero
        if (mfrc522.uid.uidByte[x] < 0x10) {
          serial += "0";
        }
        // Transform the byte to hex
        serial += String(mfrc522.uid.uidByte[x], HEX);
        // Add a hypen
        if (x + 1 != mfrc522.uid.size) {
          serial += "-";
        }
      }
      // Transform to uppercase
      serial.toUpperCase();
      lastserial = serial;
      Serial.println(serial);

      mfrc522.PICC_HaltA();  // Stop reading

      if (serial == (String)arcana.config["emergencia"]) {
        digitalWrite(D0, HIGH);
        digitalWrite(D3, HIGH);
        delay(2000);
        digitalWrite(D0, LOW);
        digitalWrite(D3, LOW);
      } 
      if (serial == (String)arcana.config["solucion"]) {
        Serial.println("Abierto!");
        isCompleted = true;
        digitalWrite(D3, HIGH);
        digitalWrite(D8, HIGH);
        delay(2000);
        digitalWrite(D8, LOW);
        delay(2000);
        digitalWrite(D3, LOW);
      } else {
        digitalWrite(D8, HIGH);
        delay(200);
        digitalWrite(D8, LOW);
        delay(200);
        digitalWrite(D8, HIGH);
        delay(200);
        digitalWrite(D8, LOW);

      }
    }
  }

  if (arcana.status == 0) {
    reset();
  }
  if (arcana.status == 1) {
    digitalWrite(D3, HIGH);
    if (!isReset) reset();
  }
  if (arcana.status == 2 && isReset) {
    isReset = false;
    if ((String)arcana.config["solucion"] == "null") {
      arcana.configurar();
      reset();
    }
  }
  if (arcana.status == 3 && !isReset) {
    arcana.configurar();
    reset();
  }



  String estado = "tarjeta=" + lastserial;
  estado += "&abierto=" + (isCompleted ? String("si") : String("no"));
  estado += "&reset=" + (isReset ? String("si") : String("no"));
  estado += "&solucion=" + (String)arcana.config["solucion"];
  estado += "&status=" + (String)arcana.status;

  arcana.estado(estado);


  ///////////////////////////////
  delay(50);
  counter--;
}

void reset() {
  isReset = true;
  isCompleted = false;
  isWaiting = false;
  serial = "";
  digitalWrite(D0, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D8, LOW);
}
