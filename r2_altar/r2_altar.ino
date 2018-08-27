#include <Arcana.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <MFRC522.h>



Arcana arcana("altar", false, true);

constexpr uint8_t RST_PIN = 5;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 4;         // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

Adafruit_NeoPixel strip = Adafruit_NeoPixel(94, 0, NEO_GRB + NEO_KHZ800);

int cardIDs[5] = {0, 0, 0, 0, 0};
bool cardActive[5] = {false, false, false, false, false};
int counter = 0;
uint8_t i = 0;
uint8_t j = 0;
bool isReset = false;
bool isOpen = false;
uint8_t fase = 0;
int targetPos = 0;
int currentPos = 0;
float factor = 0;
int angle = 0;

void setup() {
  strip.begin();
  strip.setBrightness(250);

  Serial.begin(115200);
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  reset();

  arcana.init();
  delay(50);

  String myconfig = arcana.config;
  i = 0;
  while (myconfig.indexOf(" ") > -1) {
    cardIDs[i] = myconfig.substring(0, myconfig.indexOf(" ")).toInt();
    myconfig = myconfig.substring(myconfig.indexOf(" ") + 1);
    i++;
  }
  cardIDs[i] = myconfig.toInt();

  Serial.println("Tarjetas");
  for ( i = 0; i < 5; i++) {
    Serial.println(cardIDs[i]);
  }

  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

}
void loop() {
  String estado = "";
  if (counter == 0) {
    arcana.conectar();
    counter = 25 ;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.substring(1, 4).equals("off") && !isReset) {
    reset();
  }
  if (arcana.respuesta.substring(1, 3).equals("on") && !isReset) {
    reset();
  }

  if (mfrc522.PICC_IsNewCardPresent() && fase < 5) {
    Serial.println("New card");
    int sum = 0;
    if (mfrc522.PICC_ReadCardSerial()) {
      for ( i = 0; i < 4; i++) {
        sum += mfrc522.uid.uidByte[i];
      }
      mfrc522.PICC_HaltA(); // Stop reading

      if (cardIDs[fase] == sum) {
        cardActive[i] = true;
        fase ++;
        isReset = false;
        //        for (j = 0; j < 28; j++) {
        //          for (i = 0; i < strip.numPixels(); i++) {
        //            if (i == j || i == 82 - j) {
        //              strip.setPixelColor(i, 255, 255, 255);
        //            } else {
        //              strip.setPixelColor(i, 0, 0, 0);
        //            }
        //          }
        //          strip.show();
        //          delay(10);
        //        }
        //        angle = 0;
        if (fase == 5) {
          isOpen = true;
          estado += "&Portal=abierta";
          estado += "&Fase=" + String(fase);
          arcana.estado(estado);
          arcana.conectar();
          //          rainbow(20);
        }
      } else {
        if (fase > 0 && sum != cardIDs[fase - 1] ) {
          fase = 0;
        }
      }
    }
  }

  if (arcana.respuesta.charAt(23) == '1' && !isOpen) {
    for ( i = 0; i < 5; i++) {
      cardActive[i] = true;
    }
    fase = 5;
    isOpen = true;
    rainbow(20);
    isReset = false;
  }
  if (arcana.respuesta.charAt(23) == '0' && isOpen) {
    reset();
  }
  
  if (arcana.respuesta.charAt(24) == '1') {
    isReset = false;
    fase = 6;
  }

  //  targetPos = (fase > 0) ? (fase - 1) * 60 : 0;
  switch (fase) {
    case 0:
      targetPos = 0;
      break;
    case 1:
      targetPos = 9;
      break;
    case 2:
      targetPos = 19;
      break;
    case 3:
      targetPos = 29;
      break;
    case 4:
      targetPos = 39;
      break;
    case 5:
    case 6:
      targetPos = 48;
      break;
  }
  if (targetPos > currentPos) currentPos++;
  if (targetPos < currentPos) currentPos--;

  angle += 10;
  angle %= 360;
  factor = sin(angle * 3.1415926535 / 180) * 0.45 + 0.55;

  switch (fase) {
    case 0:
      for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, 0, 0, 0);
      }
      break;
    case 1:
    case 2:
    case 3:
    case 4:
      iluminarPortal(Wheel(currentPos * 2));
      break;
    case 5:
      for (i = 0; i < strip.numPixels(); i++) {
        if (currentPos < 48) {
          iluminarPortal(Wheel(currentPos * 2));
        } else {
          strip.setPixelColor(i, 255 * factor, 255 * factor, 255 * factor);
        }
      }
      break;
    case 6:
      for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, 0, 0, 0);
      }
      break;
  }
  strip.show();

  estado = "";
  estado += "&Portal=" + (isOpen ? String("abierta") : String("cerrada"));
  estado += "&Fase=" + String(fase);
  arcana.estado(estado);

  ///////////////////////////////
  delay(40);
}

void reset() {
  for ( i = 0; i < 5; i++) {
    cardActive[i] = false;
  }
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show(); // Initialize all pixels to 'off'
  isReset = true;
  isOpen = false;
  fase = 0;
  targetPos = 0;
  currentPos = 0;
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
    return strip.Color((255 - WheelPos * 3) * factor, 0, WheelPos * 3 * factor);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3 * factor, (255 - WheelPos * 3) * factor);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3 * factor, (255 - WheelPos * 3) * factor, 0);
}

void iluminarPortal(uint32_t color) {
  for (i = 0; i < strip.numPixels(); i++) {
    if (i < 37) {
      if (i < currentPos - 11) {
        strip.setPixelColor(i, color);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    } else if (i < 85) {

      if (i > 84 - currentPos) {
        strip.setPixelColor(i, color);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    } else {
      if (i < currentPos + 85) {
        strip.setPixelColor(i, color);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    }
  }
}

