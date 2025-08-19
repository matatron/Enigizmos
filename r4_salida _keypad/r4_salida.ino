#define LED_BUILTIN 2
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define TX 1
#define RX 3
#define SD2 9
#define SD3 10
#include <Arcana3.h>
#include <Key.h>
#include <Keypad.h>

Arcana3 arcana("salida", false);

int counter = 0;
bool isReset = false;
bool isCompleted = false;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','_'}
};

char pressedKeys[9] = "________";

byte colPins[COLS] = {D5, D7, D3};
byte rowPins[ROWS] = {D6, D0, D2, D4};
int i;
 
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

void setup() {
  //Serial.begin(9600);
  //while (!Serial);
  pinMode(D1, OUTPUT);    // LED pin as output.
  pinMode(TX, OUTPUT);    // LED pin as output.
  reset();
  arcana.init();
  delay(50);
}

void loop() {
  if (counter <= 0) {
    arcana.conectar();
    counter = 30;
  }
  if(arcana.acertijo((int) arcana.config["acertijo"]) && !isCompleted) {
    isCompleted = true;
    digitalWrite(TX, HIGH);
    digitalWrite(D1, HIGH);
    delay(200);
    digitalWrite(D1, LOW);
  }

  if(!isCompleted) {
    char myKey = myKeypad.getKey();

    if (myKey != NULL){
      for(i=0; i<7; i++) {
        pressedKeys[i] = pressedKeys[i+1];
      }
      pressedKeys[7] = myKey;
      
      //Serial.println(myKey);

      if(String(pressedKeys) == (String) arcana.config["solucion"]) {
        isCompleted = true;
        digitalWrite(TX, HIGH);
      }
      digitalWrite(D1, HIGH);
      delay(200);
      digitalWrite(D1, LOW);

    }
  }

  if (arcana.status == 0 && !isReset) {
    reset();
  }
  if (arcana.status == 1 && !isReset) {
    reset();
  }
  if (arcana.status == 2 && isReset) {
    //arcana.configurar();
    isReset = false;
  }
  if (arcana.status == 3 && !isReset) {
    reset();
  }

  String estado = "pressed=" + String(pressedKeys);
  estado += "&escape=" + (isCompleted ? String("completo") : String ("pendiente"));
  arcana.estado(estado);

  ///////////////////////////////
  counter --;
  delay(25);
}

void reset() {
  isReset = true;
  isCompleted =false;
  digitalWrite(TX, LOW);
  digitalWrite(D1, LOW);
}
