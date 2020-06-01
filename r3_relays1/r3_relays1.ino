#include <Arcana3.h>

Arcana3 arcana("relays1", false);

int counter = 0;
bool isReset = false;
int puzzles[6] = {0, 0, 0, 0, 0, 0};

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
    counter = 30;

    if (arcana.status == 0 && !isReset) { // juego archivado
      configurar();
      reset();
    }

  }
  counter--;

  if (arcana.status == 3) { // juego esperando
    isReset = false;
    digitalWrite(D1, HIGH); // Maquina de Humo
    digitalWrite(D2, HIGH); // Puerta 0
    digitalWrite(D3, HIGH); // Luz Azul/Roja
    digitalWrite(D4, LOW); // Luz Azul/Roja
    digitalWrite(D5, HIGH); // luz cuarto 2
    digitalWrite(D6, HIGH); // puerta 1
    digitalWrite(D7, HIGH); // luz cuarto 3
  }

  if (arcana.status == 2) {
    isReset = false;
    digitalWrite(D1, arcana.acertijo(puzzles[0])); // Maquina de Humo
    digitalWrite(D2, !arcana.acertijo(puzzles[1])); // Puerta 0
    digitalWrite(D3, !arcana.acertijo(puzzles[2])); // Luz Azul
    digitalWrite(D4, !arcana.acertijo(puzzles[2])); // Roja
    digitalWrite(D5, !arcana.acertijo(puzzles[3])); // luz cuarto 2
    digitalWrite(D6, !arcana.acertijo(puzzles[4])); // puerta 1
    digitalWrite(D7, !arcana.acertijo(puzzles[5])); // luz cuarto 3
  }

  if (arcana.status == 1) { // juego esperando
    isReset = false;
    digitalWrite(D1, HIGH); // Maquina de Humo
    digitalWrite(D2, LOW); // Puerta 0
    digitalWrite(D3, HIGH); // Luz Azul/Roja
    digitalWrite(D4, LOW); // Luz Azul/Roja
    digitalWrite(D5, LOW); // luz cuarto 2
    digitalWrite(D6, LOW); // puerta 1
    digitalWrite(D7, LOW); // luz cuarto 3
  }

  String estado = "";
  //estado += "&Config=" + arcana.config;
  arcana.estado(estado);
  ///////////////////////////////
  delay(20);
}

void reset() {
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH); // Maquina de Humo
  digitalWrite(D2, HIGH); // Puerta 0
  digitalWrite(D3, HIGH); // Luz Azul/Roja
  digitalWrite(D4, LOW); // Luz Azul/Roja
  digitalWrite(D5, LOW); // luz cuarto 2
  digitalWrite(D6, HIGH); // puerta 1
  digitalWrite(D7, LOW); // luz cuarto 3
  digitalWrite(D8, HIGH);
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
