#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(60, 8, NEO_GRB + NEO_KHZ800);


int angle = 0;
int angle2 = 0;

void setup() {

  Serial.begin(115200);

  strip1.begin();
  strip1.setBrightness(255);
  strip1.show(); // Initialize all pixels to 'off'
}

void loop() {
  angle += 8;
  angle %= 256;
  angle2 += 4;
  angle2 %= 256;
  uint16_t i = 0;
  uint16_t j = 120;
  float factor;

  Serial.println(angle);
  for (i = 0; i < 30; i++) {
//    j = 120-4*i;
//    j = 4*i;
    j = 60+60*cos(3.1415926535 * (angle2+i*30) / 128);
    factor = 0.5 + cos(3.1415926535 * (angle-i*20) / 128) * 0.5;
    strip1.setPixelColor(29 - i, factor * (250-j), 20 * factor, factor * (85+j));
    strip1.setPixelColor(30 + i, factor * (250-j), 20 * factor, factor * (85+j));
  }
  strip1.show();

  delay(20);
}

