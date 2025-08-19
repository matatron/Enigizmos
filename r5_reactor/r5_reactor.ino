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


Arcana3 arcana("reactor", true);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, D3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(150, D5, NEO_GRB + NEO_KHZ800);


int counter = 0;
bool isReset = false;
bool isCompleted = false;
uint8_t i;
unsigned long pressedTime = 0;


void setup() {
  Serial.begin(115200);
  while (!Serial);
  reset();
  arcana.init();
  delay(50);
  strip.begin();
  for ( i = 0; i < 60; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show(); // Initialize all pixels to 'off'
  strip2.begin();
  for ( i = 0; i < 150; i++) {
    strip2.setPixelColor(i, 0, 0, 0);
  }
  strip2.show();
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
}

void loop() {
  if (counter <= 0) {
    arcana.conectar();
    counter = 20;
  }
  uint8_t alpha = 127*(1+cos((float) millis()/320));

  if(arcana.status == 2 && !isCompleted) {

    if(arcana.acertijo((int) arcana.config["acertijo"])) {
      isCompleted = true;
      counter = 20;
      Serial.println("Activado remoto!");
    }

    if (digitalRead(D1) == HIGH) {
      pressedTime = 0;
      for ( i = 0; i < 60; i++) {
        strip.setPixelColor(i, 0, 0, 0);
      }
      strip.show(); 
      for ( i = 0; i < 150; i++) {
        strip2.setPixelColor(i, 0, 0, 0);
      }
      strip2.show();
    }else { 
      if (digitalRead(D2) == HIGH) {
        for ( i = 0; i < 40; i++) {
          strip.setPixelColor(i, 80-i*2, 0, 0);
        }
        strip.show();
        pressedTime = 0;
      } else {
        if (pressedTime == 0) {
          for ( i = 0; i < 60; i++) {
            strip.setPixelColor(i, 0, 0, 0);
          }
          for ( i = 0; i < 150; i++) {
            strip2.setPixelColor(i, 0, 0, 0);
          }
          pressedTime = millis();
        }
      }
    }

    if (pressedTime!= 0 && !isCompleted) {
      uint8_t timePassed = (millis() -pressedTime) / 400;
      if (timePassed >= 150) {
        isCompleted = true;
        counter = 0;
        Serial.println("Activado manual!");
      } else {
        for ( i = 0; i < timePassed/2.5; i++) {
          strip.setPixelColor(i, 0, 0, alpha);
        }
        strip.show(); 
        for ( i = 0; i < timePassed; i++) {
          strip2.setPixelColor(i, 0, 200, 0);
        }
        strip2.setPixelColor(i, 0, ((millis() -pressedTime)%400)/2, 0);
        strip2.show();
      }
    }
  }
  if (arcana.status == 2 && isCompleted) {
    for ( i = 0; i < 60; i++) {
      strip.setPixelColor(i, alpha,alpha,alpha);
    }
    strip.show(); 
    for ( i = 0; i < 150; i++) {
      strip2.setPixelColor(i, 0, 200, 0);
    }
    strip2.show();
    
  }
  if (arcana.status == 1 && !isReset) {
    reset();
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
  estado += "&presionado=" + (pressedTime != 0 ? String("si") : String ("no"));
  estado += "&puerta=" + (digitalRead(D2) == LOW ? String("cerrada") : String ("abierta"));
  
  estado += "&reset=" + (isReset ? String("si") : String ("no"));
  estado += "&status=" + (String) arcana.status;
  arcana.estado(estado);


  ///////////////////////////////
  delay(50);
  counter --;
}

void reset() {
  isReset = true;
  isCompleted = false;
  pressedTime = 0;
  for ( i = 0; i < 60; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show(); // Initialize all pixels to 'off'
  for ( i = 0; i < 150; i++) {
    strip2.setPixelColor(i, 0, 0, 0);
  }
  strip2.show();
}
