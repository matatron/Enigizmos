#include <Arcana.h>
#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"

Arcana arcana("tormenta", true, true);

#define SFX_TX 12
#define SFX_RX 13
#define SFX_RST 14


SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

uint8_t n = 0;
bool hasSFX = false;
unsigned long thundertime;
unsigned int thunderN;
int counter = 0;

bool isReset = false;
bool isAnim1 = false;
bool isAnim2 = false;
bool isAnim3 = false;
bool isAnim4 = false;
bool isAnim5 = false;
bool isAnim6 = false;
bool isAnim7 = false;
bool isAnim8 = false;
bool oscuridad = LOW;

Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, 16);


void setup() {
  pinMode(D0, OUTPUT); //
  pinMode(D1, OUTPUT); //
  pinMode(D2, OUTPUT); // rayos
  pinMode(D3, OUTPUT); // luz habitacion
  reset();

  arcana.init();
  delay(50);

  ss.begin(9600);
  sfx.reset();
  digitalWrite(SFX_RST, LOW);
  pinMode(SFX_RST, OUTPUT);
  delay(10);
  pinMode(SFX_RST, INPUT);
  delay(1000);

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
  if (arcana.respuesta.toInt() == 0 && !isReset) {
    reset();
  }

  // sustos
  if (arcana.respuesta.charAt(4) == '1' && !isAnim1) {
    isReset = false;
    isAnim1 = true;
    flicker(false);
    flicker(false);
  }

  //anabelle
  if (arcana.respuesta.charAt(5) == '1' && !isAnim2) {
    isReset = false;
    isAnim2 = true;
    lucesOff(false);
    delay(3000);
    lucesOn(false);
  }

  //video 20s
  if (arcana.respuesta.charAt(6) == '1' && !isAnim3) {
    isReset = false;
    isAnim3 = true;
    lucesOff(false);
    delay(22000);
    lucesOn(false);
  }

  if (arcana.respuesta.charAt(7) == '1' && !isAnim4) {
    isReset = false;
    isAnim4 = true;
    flicker(false);
    flicker(false);
  }


  if (arcana.respuesta.charAt(8) == '1' && !isAnim5) {
    isReset = false;
    isAnim5 = true;
    playThunder(5);
  }


  if (arcana.respuesta.charAt(9) == '1' && !isAnim6) {
    isReset = false;
    isAnim6 = true;
    flicker(false);
  }

  //piano
  if (arcana.respuesta.charAt(15) == '1' && !isAnim7) {
    isReset = false;
    isAnim7 = true;
    lucesOff(false);
    delay(6000);
    lucesOn(false);
  }

  if (arcana.respuesta.charAt(21) == '1' && !isAnim8) {
    isReset = false;
    isAnim8 = true;
    flicker(false);
    oscuridad = HIGH;
    lucesOff(false);
    playThunder(8);
  }
  
  if (arcana.respuesta.charAt(23) == '1') {
    isReset = false;
    digitalWrite(D2, LOW);
  } else if (arcana.respuesta.charAt(1) == '1') {
    if (millis() > thundertime) {
      thunderN = random(0, 14);
      playThunder(thunderN);
    }
  }

  if (arcana.respuesta.charAt(24) == '1') {
    digitalWrite(D2, HIGH);
    oscuridad = LOW;
  }

  String estado = "";
  if (thundertime > millis()) {
    estado += "&RayoEn=" + String(floor((thundertime - millis()) / 1000));
  } else {
    estado += "&RayoEn=esperando";
  }
  estado += "&UltimoRayo=" + String(thunderN);
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  thundertime = millis();
  isReset = true;
  oscuridad = LOW;
  isAnim1 = false;
  isAnim2 = false;
  isAnim3 = false;
  isAnim4 = false;
  isAnim5 = false;
  isAnim6 = false;
  isAnim7 = false;
  isAnim8 = false;
}

void playThunder(int n) {
  thundertime = millis() + random(30000, 120000);
  if (isAnim8)   thundertime = millis() + random(30000, 60000);
  digitalWrite(D2, LOW);
  delay(700);
  sfx.playTrack(n);
  sfx.volUp();
  digitalWrite(D2, HIGH);
  flicker(true);
}

void lucesOff(bool rayos) {
  if (rayos) digitalWrite(D2, LOW);
  digitalWrite(D1, HIGH);
  digitalWrite(D3, HIGH);
}
void lucesOn(bool rayos) {
  if (rayos) digitalWrite(D2, HIGH);
  digitalWrite(D1, oscuridad);
  digitalWrite(D3, oscuridad);
}

void flicker(bool rayos) {
  int n = random(5, 10);
  while (n > 0) {
    n--;
    delay(random(50, 300));
    lucesOff(rayos);
    delay(random(50, 500));
    lucesOn(rayos);
  }
}

