#include <Arcana3.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

Arcana3 arcana("adn", true);

int counter = 0;
bool isReset = false;
bool isAlarmOff = false;
bool isDNADone = false;
bool scanningDNA = false;

unsigned long timeAlarma = 0;
unsigned long timeVenom = 0;
unsigned long timeDNA = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(7, D3, NEO_GRB + NEO_KHZ800);

void setup() {
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pinMode(D0, OUTPUT); //botones
  pinMode(D1, OUTPUT); //botones
  pinMode(D2, OUTPUT); //botones
  pinMode(D4, OUTPUT); //botones
  pinMode(D5, INPUT_PULLUP); //botones
  pinMode(D6, INPUT_PULLUP); //botones
  pinMode(D7, INPUT_PULLUP); //botones
  Serial.begin(115200);

  strip.begin(); // This initializes the NeoPixel library.
  reset();

  arcana.init();
  delay(50);
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50;

    if (arcana.status == 0 && !isReset) {
      arcana.configurar();
      reset();
    }

    if (arcana.status == 1 && !isReset) {
      reset();
    }

  }
  counter--;

  if (arcana.status == 2) {
    isReset = false;
    if (!arcana.acertijo(3)) {
      digitalWrite(D2, (millis() % 2000 > 1000)); // Luz alarma
    } else {
      digitalWrite(D2, LOW);
    }

    if (digitalRead(D5) == LOW && !isAlarmOff) {
      isAlarmOff = true;
      counter = 0;
    }
    if (digitalRead(D6) == HIGH) {
      if (!scanningDNA) {
        scanningDNA = true;
        counter = 0;
        timeDNA = millis();
      }
      if (scanningDNA && !isDNADone) {
        unsigned long timePass = millis() - timeDNA;
        for (int i = 0; i < 7; i++) {
          if (i < (timePass / 1000) % 8 + 1) {
            strip.setPixelColor(i, strip.Color(100, 0, 0));
          } else {
            strip.setPixelColor(i, strip.Color(0, 0, 0));
          }
          /*          if (i == (millis() / 50) % 6) {
                      strip.setPixelColor(i + 1, strip.Color(150, 0, 0));
                    } else {
                      strip.setPixelColor(i + 1, strip.Color(0, 0, 0));
                    }*/
        }
        strip.show(); // This sends the updated pixel color to the hardware.

        if (timePass > 7000) {
          isDNADone = true;
          counter = 0;

          for (int i = 0; i < 7; i++) {
            strip.setPixelColor(i, strip.Color(0, 150, 0)); // Moderately bright green color.
          }
          strip.show(); // This sends the updated pixel color to the hardware.
          delay(1000);
          for (int i = 0; i < 7; i++) {
            strip.setPixelColor(i, strip.Color(0, 0, 0));
          }
          strip.show(); // This sends the updated pixel color to the hardware.
        }
      }
    } else {
      scanningDNA = false;
      for (int i = 0; i < 7; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      strip.show(); // This sends the updated pixel color to the hardware.
    }
  }

  String estado = "";
  estado += "&Alarma=" + (isAlarmOff ? String("apagada") : String("encendida"));
  estado += "&ADN=" + (isDNADone ? String("humano") : (scanningDNA ? String("escaneando") : String("desconocido")));
  arcana.estado(estado);


  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  isAlarmOff = false;
  isDNADone = false;
  isReset = true;
  for (int i = 0; i < 7; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show(); // This sends the updated pixel color to the hardware.
}
