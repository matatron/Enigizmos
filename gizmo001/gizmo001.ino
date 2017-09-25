#include <ESP8266WiFi.h>
#define LED_BUILTIN 2
#define SENSORLASER 5
#define INTENSILEDS 12
#define RELAY1 13
#define RELAY2 15
#define BOTON1 14
#define libre1 4 //d7
#define nousar 8 //d8

const char* ssid = "Enigmata";
const char* password = "2.718281";
const char* host = "192.168.0.83";
const char* gizmoName = "gizmo1";

int espera = 1000;
String respuesta = "";
String estado = "";
int counter = 0;
int intensidad = 0;

String forja = "apagada";
String rayo = "no";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(INTENSILEDS, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, LOW);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY2, LOW);
  pinMode(SENSORLASER, INPUT);
  pinMode(BOTON1, INPUT);
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
  if (intensidad > 1000) {
    if (forja == "apagada") {
      forja = "encendida";
      analogWrite(INTENSILEDS, 1023);
//      digitalWrite(RELAY1, HIGH);
//      delay(1000);
//      digitalWrite(RELAY1, LOW);
    }
  } else {
    if (digitalRead(SENSORLASER) == HIGH) {
      intensidad += 5;
    } else {
      intensidad -= 2;
      if (intensidad < 0) intensidad = 0;
    }
    analogWrite(INTENSILEDS, intensidad);
  }
  if (digitalRead(BOTON1) == HIGH && rayo == "no") {
    rayo = "si";
    digitalWrite(RELAY2, HIGH);
    delay(1000);
    digitalWrite(RELAY2, LOW);
  }
  Serial.println(intensidad);

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
  url += "/?intensidad=";
  url += intensidad;
  url += "&forja=";
  url += forja;
  url += "&rayo=";
  url += rayo;
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
