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
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>


Arcana3 arcana("generador", true);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, D2, NEO_GRB + NEO_KHZ800);

EspSoftwareSerial::UART myPort;
DFRobotDFPlayerMini myDFPlayer;


int counter = 0;
bool isReset = false;
bool isCompleted = false;
bool resetValueBtn = false;
uint8_t i;
 

void setup() {
  myPort.begin(9600, SWSERIAL_8N1, D5, D6, false);
  Serial.begin(115200);
  while (!Serial);
  reset();
  arcana.init();
  delay(50);
  strip.begin();
  for ( i = 0; i < 60; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show(); // Initialize all pixels to 'off'
  pinMode(D1, INPUT_PULLUP);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(myPort, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    } /**/
  }
  Serial.println(F("DFPlayer Mini online."));

}

void loop() {
  if (counter <= 0) {
    arcana.conectar();
    counter = 30;
  }
  if(arcana.status == 2 && !isCompleted) {
    int sensorVal = (digitalRead(D1) == HIGH);
    if(arcana.acertijo((int) arcana.config["acertijo"])) {
      isCompleted = true;
      counter = 30;
      digitalWrite(D3, HIGH);
      digitalWrite(D4, LOW);
      Serial.println("Activado remoto!");
    }
    if (sensorVal != resetValueBtn) {
      isCompleted = true;
      counter = 30;
      digitalWrite(D3, HIGH);
      digitalWrite(D4, LOW);
      Serial.println("Activado manual!");
    }
  }
  for ( i = 0; i < 30; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip.setPixelColor(59-i, 0, 0, 0);
    if(isCompleted && counter < 22) {
      strip.setPixelColor(30+counter, 200, 200, 250);
      strip.setPixelColor(29-counter, 200, 200, 250);
    }
  }
  strip.show(); 
  if(isCompleted && counter == 30) {
    myDFPlayer.play(1);  //Play the first mp3
    delay(300);
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

  String estado = "activo=" + (isCompleted ? String("si") : String ("no"));
  estado += "&reset=" + (isReset ? String("si") : String ("no"));
  estado += "&status=" + (String) arcana.status;
  arcana.estado(estado);


  ///////////////////////////////
  delay(50);
  counter --;
}

void reset() {
  isReset = true;
  isCompleted = false;
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
  resetValueBtn = (digitalRead(D1) == HIGH);
  for ( i = 0; i < 60; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show(); // Initialize all pixels to 'off'

}
