#include <Arcana3.h>

Arcana3 arcana("relays2", true);

int counter = 0;
bool isReset = false;
bool isAlienOn = false;
int puzzles[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  pinMode(D0, OUTPUT); //botones
  pinMode(D1, OUTPUT); //botones
  pinMode(D2, OUTPUT); //botones
  pinMode(D3, OUTPUT); //botones
  pinMode(D4, OUTPUT); //botones
  pinMode(D5, OUTPUT); //botones
  pinMode(D6, OUTPUT); //botones
  pinMode(D7, OUTPUT); //botones
  pinMode(D8, OUTPUT); //botones
  Serial.begin(115200);

  reset();

  arcana.init();
  delay(10);
  configurar();
  delay(10);
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 75;

    if (arcana.status == 0 && !isReset) { // juego archivado
      configurar();
      reset();
    }

  }
  counter--;

  if (arcana.status == 3) { // juego finalizado
    isReset = false;
    digitalWrite(D2, LOW); // ALIEN
    digitalWrite(D3, HIGH); // LUZ 1
    digitalWrite(D5, HIGH); // LUZ 2
    digitalWrite(D6, HIGH); // puerta 2
    digitalWrite(D7, HIGH); // mano alien
  }


  if (arcana.status == 2) {
    isReset = false;
    if (arcana.acertijo(puzzles[0]) && !isAlienOn) {
      isAlienOn = true;
      delay(4000);
      digitalWrite(D3, LOW); // Luz 1
      delay(2000);
      digitalWrite(D5, LOW); // Luz 1
      delay(2000);
      digitalWrite(D2, HIGH); // Luz 1
    }
    if (!arcana.acertijo(puzzles[0]) && isAlienOn) {
      isAlienOn = false;
      digitalWrite(D3, HIGH); // Luz 1
      digitalWrite(D5, HIGH); // Luz 1
      digitalWrite(D2, LOW); // Luz 1
    }

    if (arcana.acertijo(puzzles[1])) {
      digitalWrite(D3, HIGH); // Luz 1
      digitalWrite(D5, HIGH); // Luz 1
    }
    
    digitalWrite(D6, !arcana.acertijo(puzzles[2])); // puerta 1
    digitalWrite(D7, !arcana.acertijo(puzzles[3])); // puerta 1
    digitalWrite(D1, !arcana.acertijo(puzzles[4])); // puerta 1
  }

  if (arcana.status == 1) { // juego finalizado
    isReset = false;
    digitalWrite(D2, HIGH); // ALIEN
    digitalWrite(D3, HIGH); // LUZ 1
    digitalWrite(D5, HIGH); // LUZ 2
    digitalWrite(D6, HIGH); // puerta 2
    digitalWrite(D7, HIGH); // mano alien
  }

  String estado = "";
  estado += "&Config=" + arcana.config;
  arcana.estado(estado);

  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);
  isAlienOn = false;
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
