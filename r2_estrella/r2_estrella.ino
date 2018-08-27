#include <Arcana.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <MFRC522.h>



Arcana arcana("estrella", true, true);

constexpr uint8_t RST_PIN = 5;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 4;         // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, 0, NEO_GRB + NEO_KHZ800);

int cardIDs[5] = {0, 0, 0, 0, 0};
bool cardActive[5] = {false, false, false, false, false};
int counter = 0;
uint8_t i = 0;
bool isReset = false;
bool isOpen = false;
int ultimaTarjeta = 0;

void setup() {
  pinMode(D0, OUTPUT); //RELAY
  strip.begin();
  for ( i = 1; i < 5; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.setPixelColor(0, 250, 0, 0);
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(115200);
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  reset();
  strip.setPixelColor(1, 250, 0, 0);
  strip.show(); // Initialize all pixels to 'off'

  arcana.init();
  delay(50);
  strip.setPixelColor(2, 250, 0, 0);
  strip.show(); // Initialize all pixels to 'off'

  String myconfig = arcana.config;
  i = 0;
  while (myconfig.indexOf(" ") > -1) {
    cardIDs[i] = myconfig.substring(0, myconfig.indexOf(" ")).toInt();
    myconfig = myconfig.substring(myconfig.indexOf(" ") + 1);
    i++;
  }
  cardIDs[i] = myconfig.toInt();
  if (i > 2) {
    strip.setPixelColor(3, 250, 0, 0);
  } else {
    strip.setPixelColor(3, 0, 250, 0);
  }
  strip.show(); // Initialize all pixels to 'off'

  Serial.println("Tarjetas");
  for ( i = 0; i < 5; i++) {
    Serial.println(cardIDs[i]);
  }

  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

  strip.setPixelColor(4, 250, 0, 0);
  strip.show(); // Initialize all pixels to 'off'

}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50 ;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.substring(1, 4).equals("off") && !isReset) {
    reset();
  }
  if (arcana.respuesta.substring(1, 3).equals("on") && !isReset) {
    reset();
  }

  if (mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("New card");
    ultimaTarjeta = 1;
    int sum = 0;
    if (mfrc522.PICC_ReadCardSerial()) {
      for ( i = 0; i < 4; i++) {
        sum += mfrc522.uid.uidByte[i];
      }
      mfrc522.PICC_HaltA(); // Stop reading
      ultimaTarjeta =  sum;
      for ( i = 0; i < 5; i++) {
        if (cardIDs[i] == sum) {
          cardActive[i] = true;
          counter = 0;
        }
      }
    }
  }

  //  if (arcana.respuesta.charAt(16) == '0' && cardActive[0]) {
  //    cardActive[0] = false;
  //    isReset = false;
  //  }
  if (arcana.respuesta.charAt(16) == '1' && !cardActive[0]) {
    cardActive[0] = true;
    isReset = false;
  }
  //  if (arcana.respuesta.charAt(17) == '0' && cardActive[1]) {
  //    cardActive[1] = false;
  //    isReset = false;
  //  }
  if (arcana.respuesta.charAt(17) == '1' && !cardActive[1]) {
    cardActive[1] = true;
    isReset = false;
  }
  //  if (arcana.respuesta.charAt(18) == '0' && cardActive[2]) {
  //    cardActive[2] = false;
  //    isReset = false;
  //  }
  if (arcana.respuesta.charAt(18) == '1' && !cardActive[2]) {
    cardActive[2] = true;
    isReset = false;
  }
  //  if (arcana.respuesta.charAt(19) == '0' && cardActive[3]) {
  //    cardActive[3] = false;
  //    isReset = false;
  //  }
  if (arcana.respuesta.charAt(19) == '1' && !cardActive[3]) {
    cardActive[3] = true;
    isReset = false;
  }
  //  if (arcana.respuesta.charAt(20) == '0' && cardActive[4]) {
  //    cardActive[4] = false;
  //    isReset = false;
  //  }
  if (arcana.respuesta.charAt(20) == '1' && !cardActive[4]) {
    cardActive[4] = true;
    isReset = false;
  }

  /*
    if (arcana.respuesta.charAt(21) == '1' && !isOpen) {
      for ( i = 0; i < 5; i++) {
        cardActive[i] = true;
      }
      isReset = false;
    }
  */

  int totalCards = 0;

  for ( i = 0; i < 5; i++) {
    if (cardActive[i]) {
      totalCards ++;
      isReset = false;
      if (isOpen) {
        strip.setPixelColor(i, 200, 0, 0);
      } else {
        switch (i) {
          case 0:
            //azul
            strip.setPixelColor(i, 255, 0, 0);
            break;
          case 1: // no azul
            //verde
            strip.setPixelColor(i, 200, 100, 0);
            break;
          case 2:
            strip.setPixelColor(i, 0, 250, 0);
            break;
          case 3:
            strip.setPixelColor(i, 0, 0, 250);
            break;
          case 4: // no verde
            strip.setPixelColor(i, 128, 0, 128);
            break;
        }
      }
    } else {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();

  String estado = "";
  estado += "&Estado=" + (isOpen ? String("abierta") : String("cerrada"));
  estado += "&Llave1=" + (cardActive[0] ? String("activada") : String("off"));
  estado += "&Llave2=" + (cardActive[1] ? String("activada") : String("off"));
  estado += "&Llave3=" + (cardActive[2] ? String("activada") : String("off"));
  estado += "&Llave4=" + (cardActive[3] ? String("activada") : String("off"));
  estado += "&Llave5=" + (cardActive[4] ? String("activada") : String("off"));
  estado += "&ultimaTarjeta=" + String(ultimaTarjeta);
  estado += "&config=" + String(arcana.config.length());

  arcana.estado(estado);

  if (arcana.respuesta.substring(16, 22).equals("111111") && !isOpen) {
    arcana.conectar();
    rainbow(50);
    digitalWrite(D0, LOW);
    isOpen = true;
    isReset = false;
  }

  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D0, HIGH);
  for ( i = 0; i < 5; i++) {
    cardActive[i] = false;
  }
  isReset = true;
  isOpen = false;
  ultimaTarjeta = 0;
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
