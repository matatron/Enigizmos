#include <Arcana.h>
#include <Adafruit_NeoPixel.h>

Arcana arcana("medusabar", true, false);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, D1, NEO_GRB + NEO_KHZ800);

bool isReset = false;
int counter = 0;
int vidaActual = 0;
int vidaObjetivo = 0;
int i;

void setup() {
  pinMode(D2, OUTPUT); //buzzer
  strip.begin();
  strip.setBrightness(128);
  for ( i = 0; i < 8; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(115200);
  reset();

  arcana.init();
  delay(50);

}
void loop() {
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
  if (arcana.respuesta.toInt() == 0 && !isReset) {
    reset();
  }

  if (arcana.respuesta.toInt() >= 14 && vidaActual == 0) {
    vidaObjetivo = 800;
    vidaActual = 800;
  }
  if (arcana.respuesta.toInt() == 15) {
    vidaObjetivo = 500;
  }
  if (arcana.respuesta.toInt() == 16) {
    vidaObjetivo = 200;
  }
  if (arcana.respuesta.toInt() >= 17) {
    vidaObjetivo = 0;
    vidaActual = 0;
  }

  if (vidaActual > vidaObjetivo) vidaActual -= 5;

  for ( i = 0; i < 8; i++) {
    if (i * 100 < vidaActual) {
      strip.setPixelColor(i, Wheel(vidaActual * 0.133333 - 26));
    } else {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();

  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D2, LOW);
  noTone(D2);
  vidaActual = 0;
  vidaObjetivo = 0;
  isReset = true;
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
