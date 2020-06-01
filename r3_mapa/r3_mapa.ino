#include <Arcana3.h>
#include <Adafruit_NeoPixel.h>

Arcana3 arcana("mapa", false);

int counter = 0;
bool isReset = false;
int puzzles[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte players = 2;
byte posX = 0;
byte posY = 0;
uint32_t blue = 0;
uint32_t green = 0;
uint32_t red = 0;
uint32_t black = 0;
uint32_t white = 0;
uint32_t fullwhite = 0;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, D3, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  strip.begin();
  black = strip.Color(0, 0, 0);
  white = strip.Color(100, 100, 100);
  fullwhite = strip.Color(255, 255, 255);
  red = strip.Color(200, 0, 0);
  green = strip.Color(0, 200, 0);
  blue = strip.Color(0, 0, 200);

  arcana.init();
  delay(50);
  configurar();
  delay(50);
  reset();
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 100;

    if (arcana.status == 0 && !isReset) {
      arcana.configurar();
      reset();
    }

    if (arcana.status == 3 && !isReset) {
      reset();
    }

  }
  counter--;

  if (arcana.status == 2 && isReset) {
      isReset = false;
      players = arcana.doc["params"]["players"];
  }
  if (arcana.status == 2 && arcana.acertijo(18)) {
      posX = arcana.doc["params"]["posX"];
      posY = arcana.doc["params"]["posY"];
      strip.clear();
      if (posX%2) {
        strip.setPixelColor((6-posX)*7+posY, blue);
      } else {
        strip.setPixelColor((6-posX)*7+(6-posY), blue);
      }
      strip.show();
  }

  String query = "";
//  query += "&Estado=" + String(estado);  
  arcana.estado(query);


  ///////////////////////////////
  delay(15);
}


void reset() {
  strip.clear();
  strip.show();
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
