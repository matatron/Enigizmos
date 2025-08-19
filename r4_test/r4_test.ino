#include <Arcana3.h>

Arcana3 arcana("test", true);

int counter = 0;
bool isReset = false;

void setup() {
  Serial.begin(115200);
  reset();
  arcana.init();
  delay(50);
}

void loop() {
    arcana.conectar();

    
    Serial.println((String) arcana.config["test1"]);
    Serial.println((String) arcana.status);
/*
    if (arcana.status == 0 && !isReset) {
      arcana.configurar();
      reset();
    }

    if (arcana.status == 1 && !isReset) {
      reset();
    }*/


  String estado = "Hola=mundo";
//  estado += "&Alarma=" + (isAlarmOff ? String("apagada") : String("encendida"));
//  estado += "&ADN=" + (isDNADone ? String("humano") : (scanningDNA ? String("escaneando") : String("desconocido")));
  arcana.estado(estado);


  ///////////////////////////////
  delay(500);
}

void reset() {
}
