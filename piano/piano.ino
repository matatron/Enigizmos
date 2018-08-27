/*
  PIANO usando pulsadores
*/

// se definen las constantes
// se declaran las frecuencia de las notas básicas
const int NOTA_DO  = 262;
const int NOTA_RE  = 294;
const int NOTA_MI  = 330;
const int NOTA_FA  = 349;
const int NOTA_SOL = 392;
const int NOTA_LA  = 440;
const int NOTA_SI  = 494;
const int NOTA_DO2  = 524;

// se declaran los pines asociados a las notas básicas
const int pulsador_DO  = 4;
const int pulsador_RE  = 5;
const int pulsador_MI  = 6;
const int pulsador_FA  = 7;
const int pulsador_SOL = 8;
const int pulsador_LA  = 9;
const int pulsador_SI  = 10;
const int pulsador_DO2  = 11;

// se define las notas que se reproducirán
int notas_basicas[ ] = {NOTA_DO, NOTA_RE, NOTA_MI, NOTA_FA, NOTA_SOL, NOTA_LA, NOTA_SI, NOTA_DO2};
int pulsadores   [ ] = {pulsador_DO, pulsador_RE, pulsador_MI, pulsador_FA,
                        pulsador_SOL, pulsador_LA, pulsador_SI, pulsador_DO2
                       };
int pulsador_activado = 0;
unsigned long nextReset = 0;

String secuencia = "";

void setup() {
  // se define los pines de entrada y salida
  for (int pin = 7; pin > 0; pin--) {
    pinMode(pulsadores[pin] , INPUT);
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.begin(9600);
  Serial.println("Piano");
}

void loop() {
  int pulsador_actual; // se declara como variable local
  // el bucle analiza el estado de los 7 pulsadores
  for (int pulsador = 0; pulsador < 8; pulsador++) {
    pulsador_actual = digitalRead(pulsadores[pulsador]); // se lee el estado del pulsador
    // si se presiono, se procede a reproducir la nota asociada
    if (pulsador_actual == HIGH) {
      tone(13, notas_basicas[pulsador]); // reproduce la nota
      // grabamos el pulsador que ha sido apretado
      if (pulsador_activado != pulsadores[pulsador]) {
        pulsador_activado = pulsadores[pulsador];

        secuencia.concat(String(pulsador));
        int start = secuencia.length() > 13 ? secuencia.length() - 13 : 0;
        secuencia = secuencia.substring(start);
        if (secuencia.equals("2242242476554")) {
          digitalWrite(2, LOW);
          nextReset = millis() + 5000;
        }
        Serial.println(secuencia);
      }
      break;
    }
  }
  delay(25);
  // su soltamos el pulsador activo apagamos cortamos el sonido
  if (digitalRead(pulsador_activado) == LOW) {
    noTone(13);
    pulsador_activado = 0;
  }
  if (nextReset < millis()) {
    digitalWrite(2, HIGH);
  }

}
