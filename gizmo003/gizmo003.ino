#include <ESP8266WiFi.h>
#define S0 15
#define S1 3
#define S2 1
#define none 9

const char* ssid = "Enigmata";
const char* password = "2.718281";
const char* host = "192.168.0.83";
const char* gizmoName = "gizmo1";

int LED[] = {16, 5, 4, 0, 2, 14, 12, 13};
int light[] = {1, 1, 1, 1, 1, 1, 1, 1};

int counter = 0;
int i;
int currentButton = none;
int lastButton = none;

String respuesta = "";
String estado = "";

void setup() {
  for (i = 0; i < 8; i ++) {
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW);
  }

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);

//  Serial.begin(115200);
  delay(10);
  //  WiFi.mode(WIFI_STA); // Modo cliente WiFi
  //  WiFi.begin(ssid, password);
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(100);
  //    digitalWrite(LED_BUILTIN, LOW);
  //    delay(100);
  //    digitalWrite(LED_BUILTIN, HIGH);
  //  }
}
void loop() {
  if (counter == 0) {
    //    conectar();
    counter = 100;
  }
  counter--;
  //  while (respuesta.substring(1, 4).equals("off")) {
  //    Serial.println("Juego OFF. Esperando 1 minuto");
  //    delay(60000);
  //    conectar();
  //  }
  ///////////////////////////////
  //if (respuesta.toInt() > 5) { }

  currentButton = none;
  for (i = 0; i < 8; i ++) {
    digitalWrite(S0, bitRead(i, 0));
    digitalWrite(S1, bitRead(i, 1));
    digitalWrite(S2, bitRead(i, 2));
    //    Serial.print(digitalRead(SIG));

    if (analogRead(A0) > 512) currentButton = i;
  }

  if (currentButton != lastButton) {
    lastButton = currentButton;

    if (currentButton != none) {
      light[currentButton] = 1 - light[currentButton];
    }
  }

  for (i = 0; i < 8; i ++) {
    digitalWrite(LED[i], light[i]);
  }

  ///////////////////////////////
  delay(20);
}

//void conectar() {
//  digitalWrite(LED_BUILTIN, LOW);
//  WiFiClient client;
//  if (!client.connect(host, 80)) {
//    return;
//  }
//  String url = "/gizmo/report/";
//  url += gizmoName;
//  url += "/?intensidad=";
//  url += intensidad;
//  url += "&forja=";
//  url += forja;
//  url += "&rayo=";
//  url += rayo;
//  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" +
//               "Connection: close\r\n\r\n");
//  unsigned long timeout = millis();
//  while (client.available() == 0) {
//    if (millis() - timeout > 5000) {
//      client.stop();
//      return;
//    }
//  }
//  while (client.available()) {
//    respuesta = client.readStringUntil('\r');
//  }
//  digitalWrite(LED_BUILTIN, HIGH);
//}
