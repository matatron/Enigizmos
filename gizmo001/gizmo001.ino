#include <ESP8266WiFi.h>
#define LED_BUILTIN 2
#define SENSORLASER 5
const char* ssid = "Enigmata";
const char* password = "2.718281";
const char* host = "192.168.0.83";
const char* gizmoName = "gizmo1";

int espera = 1000;
String respuesta = "";
String estado = "";
int counter = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(SENSORLASER, INPUT);
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA); // Modo cliente WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
void loop() {
  if (counter == 0) {
    conectar();
    counter = 100;
  }
  counter--;
  while (respuesta.substring(1, 4).equals("off")) {
    Serial.println("Juego OFF. Esperando 1 minuto");
    delay(60000);
    conectar();
  }
  ///////////////////////////////
  //if (respuesta.toInt() > 5) { }
  Serial.println(digitalRead(SENSORLASER));

  ///////////////////////////////
  delay(20);
}

void conectar() {
  digitalWrite(LED_BUILTIN, LOW);
  WiFiClient client;
  if (!client.connect(host, 80)) {
    return;
  }
  String url = "/gizmo/report/";
  url += gizmoName;
  url += "/?status=";
  url += estado;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
      return;
    }
  }
  while (client.available()) {
    respuesta = client.readStringUntil('\r');
  }
  digitalWrite(LED_BUILTIN, HIGH);
}
