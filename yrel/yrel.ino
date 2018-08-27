#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(16, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(7, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(16, 4, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int angle = 0;

void setup() {

  Serial.begin(115200);

  pinMode(9, OUTPUT);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);

  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
  strip3.begin();
  strip3.show(); // Initialize all pixels to 'off'
}

void loop() {
  angle += 4;
  angle %= 256;
  uint16_t i = 0;
  float factor;

  if (digitalRead(12) == HIGH) {
    for (i = 0; i < strip1.numPixels(); i++) {
      factor = 0.5 + cos(3.1415926535 * angle / 128) * 0.5;
      strip1.setPixelColor(i, Wheel( 150 + 100 * factor , 0.1));
      factor = 0.5 + cos(3.1415926535 * (angle + 42) / 128) * 0.5;
      strip3.setPixelColor(i, Wheel( 150 + 100 * factor , 0.1));
    }
    for (i = 0; i < strip2.numPixels(); i++) {
      factor = 0.5 + cos(3.1415926535 * (angle + 84) / 128) * 0.5;
      strip2.setPixelColor(i, Wheel( 150 + 100 * factor , 0.1));
    }

  } else {
    for (i = 0; i < strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel(((i * 256 / strip1.numPixels()) + angle) % 255, 1));
      strip3.setPixelColor(i, Wheel(((i * 256 / strip3.numPixels()) + angle) % 255, 1));
    }
    for (i = 0; i < strip2.numPixels(); i++) {
      strip2.setPixelColor(i, Wheel(angle, 1));
    }

  }

  strip1.show();
  strip2.show();
  strip3.show();

  delay(20);
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) { // 5 cycles of all colors on wheel
    strip1.show();
    strip2.show();
    strip3.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos, float intensidad) {
  float factor = (float) random(10, 100) / 100 * intensidad;

  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip1.Color((int) (255 - WheelPos * 3) * factor, 0, (int) WheelPos * 3 * factor);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, (int) WheelPos * 3 * factor, (int) (255 - WheelPos * 3) * factor);
  }
  WheelPos -= 170;
  return strip1.Color((int)WheelPos * 3 * factor, (int) (255 - WheelPos * 3) * factor, 0);
}
