
#include <Arcana.h>
#include <Servo.h>

Servo servo1;
//Servo servo2;
int counter = 0;
Arcana arcana("cajamusica", true, true);

//const int pCerrado = 10;
//const int pAbierto = 170;

bool isReset = false;
bool isMusic = false;
bool isOpen = false;

void setup() {

  servo1.attach(D1);
//  servo2.attach(D2);
  pinMode(D0, OUTPUT);
  pinMode(D2, OUTPUT);
  reset();

  Serial.begin(115200);
  arcana.init();
  delay(50);
}
void loop() {
  if (counter == 0) {
    arcana.conectar();
    counter = 50 ;
  }
  counter--;
  //    ///////////////////////////////
  if (arcana.respuesta.substring(1, 4).equals("off") && !isOpen) {
    reset();
  }
  if (arcana.respuesta.substring(1, 3).equals("on") && isOpen) {
    closeBox();
    isMusic = false;
  }

  if (arcana.respuesta.charAt(15) == '1') {
    if (isMusic) {
      openBox();
    } else {
      openBoxMusic();
    }
    isReset = false;
  } else if (arcana.respuesta.charAt(15) == '0') {
    closeBox();
  }

  String estado = "caja=" + (isOpen ? String("abierta") : String("cerrada"));
  estado += "&musica=" + (isMusic ? String("lista") : String("pendiente"));
  arcana.estado(estado);
  ///////////////////////////////
  delay(20);
}

void reset() {
  servo1.write(90);
  openBox();
  isMusic = false;
  isReset = true;
}

void openBoxMusic() {
  Serial.println("Abriendo caja");
  isMusic = true;
  isOpen = true;
  servo1.write(40);
  delay(12000);
//  servo2.write(pAbierto);
  servo1.write(90);
  digitalWrite(D0, LOW);
  digitalWrite(D2, HIGH);
}

void openBox() {
//  servo2.write(pAbierto);
  digitalWrite(D0, LOW);
  digitalWrite(D2, HIGH);
  isOpen = true;
}


void closeBox() {
  Serial.println("Cerrando caja");
//  servo2.write(pCerrado);
  digitalWrite(D0, HIGH);
  digitalWrite(D2, LOW);
  isOpen = false;
}

