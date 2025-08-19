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

Arcana3 arcana("botones", false);

int counter = 0;
bool isReset = false;
bool isCompleted = false;
bool waitBetweenLevels = false;
int nivel = 0;
uint8_t i;
String idValues = "0000000";
String correctValues = "1111111";

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("SETUP");
  reset();
  arcana.init();
  delay(50);

  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
}

void getCorrectValues() {
  switch ((int)arcana.config["people"]) {
    case 2:
      if (nivel == 0) correctValues = "0010000";
      if (nivel == 1) correctValues = "0000001";
      if (nivel == 2) correctValues = "1000000";
      if (nivel == 3) correctValues = "0000010";
      if (nivel == 4) correctValues = "1100000";
      if (nivel == 5) correctValues = "0001001";
      if (nivel == 6) complete();
      break;
    case 3:
      if (nivel == 0) correctValues = "0010000";
      if (nivel == 1) correctValues = "0000001";
      if (nivel == 2) correctValues = "0100100";
      if (nivel == 3) correctValues = "0010001";
      if (nivel == 4) correctValues = "1000110";
      if (nivel == 5) correctValues = "1101100";
      if (nivel == 6) complete();
      break;
    case 4:
      if (nivel == 0) correctValues = "0010000";
      if (nivel == 1) correctValues = "0000001";
      if (nivel == 2) correctValues = "0101000";
      if (nivel == 3) correctValues = "1010100";
      if (nivel == 4) correctValues = "0101001";
      if (nivel == 5) correctValues = "1001011";
      if (nivel == 6) correctValues = "0111110";
      if (nivel == 7) complete();
      break;
    case 5:
      if (nivel == 0) correctValues = "0010000";
      if (nivel == 1) correctValues = "0000001";
      if (nivel == 2) correctValues = "0101000";
      if (nivel == 3) correctValues = "1010100";
      if (nivel == 4) correctValues = "0101001";
      if (nivel == 5) correctValues = "1001001";
      if (nivel == 6) correctValues = "0111001";
      if (nivel == 7) correctValues = "1010101";
      if (nivel == 8) complete();
      break;
    case 6:
      if (nivel == 0) correctValues = "0010000";
      if (nivel == 1) correctValues = "0000001";
      if (nivel == 2) correctValues = "0101000";
      if (nivel == 3) correctValues = "1010100";
      if (nivel == 4) correctValues = "0100101";
      if (nivel == 5) correctValues = "1001001";
      if (nivel == 6) correctValues = "0111001";
      if (nivel == 7) correctValues = "1010101";
      if (nivel == 8) correctValues = "1101111";
      if (nivel == 9) complete();
      break;
    case 7:
      if (nivel == 0) correctValues = "0010000";
      if (nivel == 1) correctValues = "0000001";
      if (nivel == 2) correctValues = "0101000";
      if (nivel == 3) correctValues = "1010100";
      if (nivel == 4) correctValues = "0101010";
      if (nivel == 5) correctValues = "1001011";
      if (nivel == 6) correctValues = "0111001";
      if (nivel == 7) correctValues = "1101101";
      if (nivel == 8) correctValues = "1101111";
      if (nivel == 9) correctValues = "1111110";
      if (nivel == 10) complete();
      break;
    default:
      if (nivel == 0) correctValues = "0010000";
      if (nivel == 1) correctValues = "0000001";
      if (nivel == 2) correctValues = "0101000";
      if (nivel == 3) correctValues = "1010100";
      if (nivel == 4) correctValues = "0101001";
      if (nivel == 5) correctValues = "1001001";
      if (nivel == 6) correctValues = "0111001";
      if (nivel == 7) correctValues = "1001001";
      if (nivel == 8) correctValues = "0110110";
      if (nivel == 9) correctValues = "1111110";
      if (nivel == 10) correctValues = "1111111";
      if (nivel == 11) complete();
      break;
  }

}

void loop() {

  getCorrectValues();

  if (arcana.status == 2 && !isCompleted) {
    bool error = false;
    idValues[0] = !digitalRead(D1) ? '1' : '0';
    if (digitalRead(D1) == LOW && correctValues[0] == '0') error = true;
    idValues[1] = !digitalRead(D2) ? '1' : '0';
    if (digitalRead(D2) == LOW && correctValues[1] == '0') error = true;
    idValues[2] = !digitalRead(D3) ? '1' : '0';
    if (digitalRead(D3) == LOW && correctValues[2] == '0') error = true;
    idValues[3] = !digitalRead(D4) ? '1' : '0';
    if (digitalRead(D4) == LOW && correctValues[3] == '0') error = true;
    idValues[4] = !digitalRead(D5) ? '1' : '0';
    if (digitalRead(D5) == LOW && correctValues[4] == '0') error = true;
    idValues[5] = !digitalRead(D6) ? '1' : '0';
    if (digitalRead(D6) == LOW && correctValues[5] == '0') error = true;
    idValues[6] = !digitalRead(D7) ? '1' : '0';
    if (digitalRead(D7) == LOW && correctValues[6] == '0') error = true;

    if (error) {
      nivel = 0;
      Serial.println("WRONG");
    }

    if (arcana.acertijo((int)arcana.config["acertijo"])) {
      isCompleted = true;
      counter = 20;
      Serial.println("Activado remoto!");
    }
    if (arcana.acertijo(((int)arcana.config["acertijo"]) - 1) && idValues == correctValues) {
      counter = 0;
      nivel++;
      getCorrectValues();
      Serial.println("Nivel superado!");
      waitBetweenLevels = true;
    }
  }

  if (arcana.status == 0 && !isReset) {
    reset();
  }
  if (arcana.status == 1 && !isReset) {
    reset();
  }
  if (arcana.status == 2 && isReset) {
    arcana.configurar();
    isReset = false;
  }
  if (arcana.status == 3 && !isReset) {
    reset();
  }

  String estado = "activo=" + (isCompleted ? String("si") : String("no"));
  estado += "&reset=" + (isReset ? String("si") : String("no"));
  estado += "&values=" + String(idValues);
  estado += "&solucion=" + String(correctValues);
  estado += "&nivel=" + String(nivel);

  estado += "&status=" + (String)arcana.status;
  arcana.estado(estado);


  ///////////////////////////////
  delay(50);
  counter--;

  if (counter <= 0) {
    arcana.conectar();
    counter = 10;
  }
  if (waitBetweenLevels) {
    delay(2000);
    waitBetweenLevels = false;
  }
}

void complete() {
  counter = 0;
  isCompleted = true;
}

void reset() {
  Serial.println("RESET");
  isReset = true;
  isCompleted = false;
  idValues = "0000000";
  nivel = 0;
  correctValues = "1111111";
}

