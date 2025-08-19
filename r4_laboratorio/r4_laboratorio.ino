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
#include <Adafruit_NeoPixel.h>

Arcana3 arcana("rfid", false);

int counter = 0;
bool isReset = false;
bool isCompleted = false;
bool isWaiting = false;
uint8_t i;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, RX, NEO_GRB + NEO_KHZ800);
uint8_t halfseconds = 0;
unsigned long errorTime = 0;

constexpr uint8_t RST_PIN = D1;    // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D2;     // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
String serial = "";
int ultimaTarjeta = 0;
bool alreadyBeep = false;

void setup() {
  pinMode(D3, OUTPUT);  // LED pin as output.
  pinMode(D8, OUTPUT);  // LED pin as output.
  Serial.begin(115200);
  while (!Serial);
  SPI.begin();
  reset();
  arcana.init();
  delay(50);
  //Serial.println("Solucion:");
  //Serial.println((String)arcana.config["solucion"]);
  //Serial.println((String)arcana.config["solucion2"]);
  //Serial.println((String)arcana.config["emergencia"]);
  mfrc522.PCD_Init();                 // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  strip.begin();
  strip.setBrightness(100);
  for (i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
  alreadyBeep = false;
  isWaiting = false;
}

void loop() {
  if (counter <= 0) {
    arcana.conectar();
    counter = 20;
  }

  if (isWaiting) {
    halfseconds = ((millis() - errorTime) / 500);
    strip.setPixelColor(0, strip.Color(150, 0, 0));
    strip.setPixelColor(1, strip.Color(150, 0, 0));
    strip.setPixelColor(2, strip.Color(150, 0, 0));
    for (int i = 0; i < 1 + halfseconds / 12; i++) {  // For each pixel...
      strip.setPixelColor(i + 3, strip.Color((halfseconds % 2) * 100, (halfseconds % 2) * 100, 0));
    }
    strip.show();  // Send the updated pixel colors to the hardware.
    if (halfseconds % 4 == 1 && !alreadyBeep) {
      digitalWrite(D8, HIGH);
      delay(50);
      digitalWrite(D8, LOW);
      alreadyBeep = true;
    }
    if (halfseconds % 4 == 0) {
      alreadyBeep = false;
    }
    if (halfseconds > 118) {
      isWaiting = false;
      errorTime = millis();
      digitalWrite(D8, HIGH);
      delay(500);
      digitalWrite(D8, LOW);
      for (int i = 0; i < 16; i++) {  // For each pixel...
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      strip.show();  // Send the updated pixel colors to the hardware.
    }

  } else if (!isCompleted) {
    if (arcana.acertijo((int)arcana.config["acertijo"])) {
      //Serial.println("Abierto!");
      digitalWrite(D3, HIGH);
      isCompleted = true;
      for (i = 0; i < 16; i++) {
        strip.setPixelColor(i, 0, 200, 0);
      }
      strip.show();  // Initialize all strip to 'off'
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(50);
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
    }
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


        mfrc522.PICC_HaltA();  // Stop reading

        if (serial == (String)arcana.config["emergencia"]) {
          digitalWrite(D3, HIGH);
          for (i = 0; i < 16; i++) {
            strip.setPixelColor(i, 0, 0, 200);
          }
          strip.show();  // Initialize all strip to 'off'
          delay(2000);
          digitalWrite(D3, LOW);
          for (int i = 0; i < 16; i++) {  // For each pixel...
            strip.setPixelColor(i, strip.Color(0, 0, 0));
          }
          strip.show();  // Initialize all strip to 'off'
        } else {
          bool siAbre = false;
          if (serial == (String)arcana.config["solucion"]) siAbre = true;
          if (serial == (String)arcana.config["solucion2"]) siAbre = true;
          if (siAbre) {
            //Serial.println("Abierto!");
            digitalWrite(D3, HIGH);
            isCompleted = true;
            for (i = 0; i < 16; i++) {
              strip.setPixelColor(i, 0, 200, 0);
            }
            strip.show();  // Initialize all strip to 'off'
            digitalWrite(D8, HIGH);
            delay(100);
            digitalWrite(D8, LOW);
            delay(50);
            digitalWrite(D8, HIGH);
            delay(100);
            digitalWrite(D8, LOW);
          } else {
            digitalWrite(D8, HIGH);
            delay(400);
            digitalWrite(D8, LOW);
            errorTime = millis();
            isWaiting = true;
          }
        }
      }
    }
  }



  if (arcana.status == 0) {
    reset();
  }
  if (arcana.status == 1 && !isReset) {
    reset();
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



  String estado = "tarjeta=" + serial;
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
  digitalWrite(D3, LOW);
  digitalWrite(D8, LOW);
  for (i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();  // Initialize all strip to 'off'
}
