#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"


#define SFX_TX 12
#define SFX_RX 13

#define SFX_RST 14

SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

uint8_t n = 0;
bool hasSFX = false;

Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

void setup() {
  pinMode(2, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);
  Serial.println("Adafruit Sound Board!");
  // softwareserial at 9600 baud
  ss.begin(9600);
  sfx.reset();

}


void loop() {
  Serial.print("\nPlaying track #"); Serial.println(n);
  sfx.playTrack(n);
  sfx.volUp();
  n++;
  n %= 14;
  delay(20000);
}
