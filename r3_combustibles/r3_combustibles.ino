#include <Arcana3.h>
#include <Adafruit_NeoPixel.h>

Arcana3 arcana("combustibles", true);

int counter = 0;
bool isReset = false;
bool isPresd = false;
byte i = 0;
int puzzles[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, D3, NEO_GRB + NEO_KHZ800);


void setup() {
  pinMode(D1, INPUT_PULLUP);

  pinMode(D0, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  strip.begin();
  strip.setBrightness(150);
  reset();
  arcana.init();
  delay(50);
  configurar();
  delay(50);
}

void loop() {
  String combustibles = "";
  if (counter == 0) {
    arcana.conectar();
    counter = 20;

    if (arcana.status == 0 && !isReset) {
      reset();
    }
    if (arcana.status == 1) {
    }
    if (arcana.status == 3 && !isReset) {
      reset();
    }

  }
  counter--;

  if (arcana.status == 2) {
    isReset = false;

    for (i = 0; i < 4; i++) {
      setPos(i);
      combustibles += (digitalRead(D1)) ? String("0") : String("1");
    }

    digitalWrite(D0, (combustibles == "1111"));
      
    if (arcana.acertijo(puzzles[0])) {
      int pressed = 0;
      for (i = 4; i < 12; i++) {
        setPos(i);
        if (digitalRead(D1) == LOW) {
          pressed = i;
        }
      }
      if (pressed > 0 && !isPresd) {
        isPresd = true;

        switch (pressed) {
          case 4:
            c1++;
            break;
          case 5:
            c2++;
            break;
          case 6:
            c3++;
            break;
          case 7:
            c4++;
            break;
          case 8:
            c1--;
            break;
          case 9:
            c2--;
            break;
          case 10:
            c3--;
            break;
          case 11:
            c4--;
            break;
        }
      }
      c1 = max(min(c1, 8), 1);
      c2 = max(min(c2, 8), 1);
      c3 = max(min(c3, 8), 1);
      c4 = max(min(c4, 8), 1);
      if (pressed == 0 && isPresd) {
        isPresd = false;
      }

    } else {
      c1 = 0;
      c2 = 0;
      c3 = 0;
      c4 = 0;
    }

    for (int i = 0; i < 8; i++) {
      if (i >= 8 - c1) {
        strip.setPixelColor(i, strip.Color(200, 0, 0));
      } else {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }
    for (int i = 8; i < 16; i++) {
      if (i < 8 + c2) {
        strip.setPixelColor(i, strip.Color(150, 150, 0));
      } else {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }
    for (int i = 16; i < 24; i++) {
      if (i >= 24 - c3) {
        strip.setPixelColor(i, strip.Color(0, 200, 0));
      } else {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }
    for (int i = 24; i < 32; i++) {
      if (i < 24 + c4) {
        strip.setPixelColor(i, strip.Color(0, 0, 200));
      } else {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }
    strip.show(); // This sends the updated pixel color to the hardware.

  }

  String estado = "";
  estado += "&Botellas=" + combustibles;
  estado += "&C1=" + String(c1);
  estado += "&C2=" + String(c2);
  estado += "&C3=" + String(c3);
  estado += "&C4=" + String(c4);
  arcana.estado(estado);


  ///////////////////////////////
  delay(5);
}

void setPos(int n) {
  digitalWrite(D7, bitRead(n, 0));
  digitalWrite(D6, bitRead(n, 1));
  digitalWrite(D5, bitRead(n, 2));
  digitalWrite(D2, bitRead(n, 3));
  delay(5);
}

void reset() {
  digitalWrite(D7, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D0, LOW);
  for (int i = 0; i < 32; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show(); // This sends the updated pixel color to the hardware.

  isReset = true;
}

void configurar() {
  arcana.configurar();
  String myconfig = arcana.config;
  int i = 0;
  while (myconfig.indexOf(" ") > -1) {
    puzzles[i] = myconfig.substring(0, myconfig.indexOf(" ")).toInt();
    myconfig = myconfig.substring(myconfig.indexOf(" ") + 1);
    i++;
  }
  puzzles[i] = myconfig.toInt();
}
