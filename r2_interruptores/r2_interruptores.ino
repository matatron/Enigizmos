#include <Arcana.h>

Arcana arcana("interruptores", false, true);

int counter = 0;
bool isReset = false;
bool isBoxOpen = false;
bool isWallOpen = false;
bool isSnakeOpen = false;


void setup() {
  pinMode(D5, INPUT_PULLUP); //caja
  pinMode(D6, INPUT_PULLUP); //botones
  pinMode(D7, INPUT_PULLUP); //serpiente

  pinMode(D1, OUTPUT); // leds
  pinMode(D2, OUTPUT); // maglock
  pinMode(D3, OUTPUT); // led caja

  Serial.begin(115200);

  reset();

  arcana.init();
  delay(50);
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50;
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

  if (digitalRead(D5) == LOW && !isBoxOpen) {
    isReset = false;
    isBoxOpen =  true;
  }
  if (arcana.respuesta.charAt(2) == '1') {
    isReset = false;
    isBoxOpen =  true;
  }
  
  if (digitalRead(D6) == LOW && !isWallOpen && isBoxOpen) {
    isReset = false;
    isWallOpen = true;
  }
  if (arcana.respuesta.charAt(3) == '1') {
    isReset = false;
    isWallOpen = true;
  }
  
  if (isBoxOpen && !isWallOpen) {
    digitalWrite(D1, LOW);
    digitalWrite(D3, HIGH);
  }
  if (digitalRead(D7) == LOW && !isSnakeOpen) {
    isReset = false;
    isSnakeOpen =  true;
    digitalWrite(D2, LOW);
  }
  if (arcana.respuesta.charAt(4) == '1') {
    isReset = false;
    isSnakeOpen =  true;
    digitalWrite(D2, LOW);
  }


  String estado = "";
  estado += "&Caja=" + (isBoxOpen ? String("abierta") : String("cerrada"));
  estado += "&Pared=" + (isWallOpen ? String("abierta") : String("cerrada"));
  estado += "&Serpiente=" + (isSnakeOpen ? String("abierta") : String("cerrada"));
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  isReset = true;
  isBoxOpen = false;
  isWallOpen = false;
  isSnakeOpen = false;
}


