// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int angle = 0;
int intensity;
//int fases[] = {0,1,2,3,4,3,2,1,2,3,4,5,6,5,4,3};
//int fases[] = {3,2,1,0,1,2,3,4,5,4,3,2,3,4,5,6};
int fases[] = {3,3,4,5,4,3,2,2,1,1,2,3,3,2,1,0};
  
void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  for(int i=0;i<NUMPIXELS;i++){
  intensity = sin(3.14159265358979*(angle-fases[i]*20)/180)*78+80;
//1111111wwwwwwwwwww    pixels.setPixelColor(i, pixels.Color(0,0,intensity));
    pixels.setPixelColor(i, pixels.Color(intensity,0,0));
 }
  angle+=2;
  if (angle>=360) angle=0;
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(10); 
}
