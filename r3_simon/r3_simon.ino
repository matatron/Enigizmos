#include <Arcana3.h>
#include <Adafruit_NeoPixel.h>

Arcana3 arcana("simon", false);

int counter = 0;
bool isReset = false;
int puzzles[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int secuencia[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int estado = 0;
int generado = 0;
int i = 0;
byte j = 0;
byte completed = 0;
byte nivel = 0;
byte buttonPressed = 5;
byte lastPressed = 5;
byte players = 2;
uint32_t color[4] = {0, 0, 0, 0};
uint32_t colorStrip[4] = {0, 0, 0, 0};
uint32_t black = 0;
uint32_t white = 0;
uint32_t fullwhite = 0;
unsigned long timer = 0;


Adafruit_NeoPixel circle = Adafruit_NeoPixel(16, D1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(44, D7, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT); //relay
  pinMode(D1, OUTPUT); //circle
  pinMode(D2, INPUT_PULLUP); //btn1
  pinMode(D3, INPUT_PULLUP); //btn2
  pinMode(D4, INPUT_PULLUP); //botones
  pinMode(D5, INPUT_PULLUP); //btn3
  pinMode(D6, INPUT_PULLUP); //btn4
  pinMode(D7, OUTPUT); //strip
  circle.begin();
  strip.begin();
  black = strip.Color(0, 0, 0);
  white = strip.Color(100, 100, 100);
  fullwhite = strip.Color(255, 255, 255);
  color[0] = circle.Color(200, 0, 0);
  color[1] = circle.Color(200, 150, 0);
  color[2] = circle.Color(0, 200, 0);
  color[3] = circle.Color(0, 0, 200);
  colorStrip[0] = strip.Color(200, 0, 0);
  colorStrip[1] = strip.Color(200, 0, 150);
  colorStrip[2] = strip.Color(0, 0, 200);
  colorStrip[3] = strip.Color(0, 200, 0);

  intro();

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
      players = arcana.doc["params"]["players"];
  }
  if (arcana.status == 2 && arcana.acertijo(15)) {
    isReset = false;
    if (estado == 0) {
      // mostrar animacion en circulo
        for (j = 0; j < 16; j++) {
          float factor = cos((j + millis()/100)*PI/8)*0.5+0.5;         
//        Serial.println(factor);
          int luz = 100*factor*factor;
          circle.setPixelColor(j, circle.Color(luz, luz, luz));
        }
        circle.show();
    }
    if (digitalRead(D4) == LOW && estado == 0) {
      digitalWrite(D0, HIGH);
      estado = 1;
      completed = 0;
      secuencia[0] = random(0, 4);
      for (i = 1; i < 9; i++) {
        secuencia[i] = (secuencia[i - 1] + random(1, 4)) % 4;
      }
      //mostrar secuencia de simón
      for (i = 0; i < calcularNivel(); i++) {
        for (j = 0; j < 16; j++) {
          circle.setPixelColor(j, black);
        }
        circle.show();
        delay(200);
        for (j = 0; j < 16; j++) {
          circle.setPixelColor(j, color[secuencia[i]]);
        }
        circle.show();
        arcana.conectar();
        delay(1000);
      }
      timer = millis() + 16000;
      buttonPressed = 5;
      lastPressed = 5;
      estado = 2;
    }
    if (estado == 2) {
      counter = 100;
      digitalWrite(D0, LOW);
      int timeLeft = timer - millis();
      for (j = 0; j < 16; j++) {
        if (j * 1000 < timeLeft) {
          if (completed == 0) {
            circle.setPixelColor(15 - (j + 18) % 16, white);
          } else {
            circle.setPixelColor(15 - (j + 18) % 16, color[secuencia[completed-1]]);
          }
        } else {
          circle.setPixelColor(15 - (j + 18) % 16, black);
        }
      }
      circle.show();

      if (millis() > timer) {
        digitalWrite(D0, HIGH);
        perdieron();
      }
    }
    if (estado == 2) {
      // mostrar tiempo en el circulo
      if (digitalRead(D2) == LOW && lastPressed != 0) {
        buttonPressed = 0;
      }
      if (digitalRead(D3) == LOW && lastPressed != 1) {
        buttonPressed = 1;
      }
      if (digitalRead(D5) == LOW && lastPressed != 2) {
        buttonPressed = 2;
      }
      if (digitalRead(D6) == LOW && lastPressed != 3) {
        buttonPressed = 3;
      }

      if (buttonPressed < 5) {
        for (j = 0; j < 44; j++) {
          strip.setPixelColor(j, colorStrip[secuencia[completed]]);
        }
        strip.show();

        if (buttonPressed == secuencia[completed]) {
          completed++;
          // animacion bien hecho
        } else {
          digitalWrite(D0, HIGH);
          delay(2000);
          //noTone(7);
          perdieron();
        }
        lastPressed = buttonPressed;
        buttonPressed = 5;
      }

      if (completed >= calcularNivel()) {
        // ganaron
        digitalWrite(D0, HIGH);
        for (j = 0; j < 44; j++) {
          strip.setPixelColor(j, fullwhite);
        }
        strip.show();
        delay(666);

        // animacion combustible llenandose
        counter = 100;
        for (byte k = 0; k < calcularNivel(); k++) {
          for (i = 0; i < 44; i++) {
            for (j = 0; j < 44; j++) {
              if (j == i) {
                strip.setPixelColor(j, fullwhite);
              } else {
                strip.setPixelColor(j, black);
              }
            }
            strip.show();
            delay(15);
          }
          generado++;
//          if (players == 2) generado++;
        }
        nivel++;
        counter = 0;
        clearPixels();
        estado = 0;

      }

      if (nivel >= 3) {
        nivel = 0;
      }
    }
    if (estado == 3) {
      clearPixels();
    }
  }

  String query = "";
  query += "&Estado=" + String(estado);
  query += "&Nivel=" + String(nivel);
  query += "&generado=" + String(generado);
  
  arcana.estado(query);


  ///////////////////////////////
  delay(15);
}

void perdieron() {
  clearPixels();
  estado = 0;
}

void clearPixels() {
  circle.clear();
  circle.show();
  strip.clear();
  strip.show();
}

void reset() {
  clearPixels();
  nivel = 0;
  estado = 0;
  completed = 0;
  generado = 0;
  digitalWrite(D0, HIGH);
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

int calcularNivel() {
  int complejidad = (nivel * 2 + 4);
  if (players == 2) complejidad = (nivel + 3);
  if (players == 3) complejidad = (nivel + 5);
  // ajustar segun las personas
  return complejidad;
}
void intro() {
  for (i = 43; i >= 0; i--) {
    for (j = 0; j < 44; j++) {
      if (j == i) {
        strip.setPixelColor(j, colorStrip[0]);
      } else {
        strip.setPixelColor(j, black);
      }
    }
    strip.show();
    delay(15);
  }
  for (i = 43; i >= 0; i--) {
    for (j = 0; j < 44; j++) {
      if (j == i) {
        strip.setPixelColor(j, colorStrip[1]);
      } else {
        strip.setPixelColor(j, black);
      }
    }
    strip.show();
    delay(15);
  }
  for (i = 43; i >= 0; i--) {
    for (j = 0; j < 44; j++) {
      if (j == i) {
        strip.setPixelColor(j, colorStrip[2]);
      } else {
        strip.setPixelColor(j, black);
      }
    }
    strip.show();
    delay(15);
  }
  for (i = 43; i >= 0; i--) {
    for (j = 0; j < 44; j++) {
      if (j == i) {
        strip.setPixelColor(j, colorStrip[3]);
      } else {
        strip.setPixelColor(j, black);
      }
    }
    strip.show();
    delay(15);
  }


}
