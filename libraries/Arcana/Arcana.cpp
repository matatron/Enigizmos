#include "Arduino.h"
#include "Arcana.h"
#include <ESP8266WiFi.h>

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

Arcana::Arcana(String gizmoName, bool LED, bool useBin)
{
    _gizmoName = gizmoName;
    _useLED = LED;
    _isBin = useBin;
    _estado = "";
    config = String("");
    respuesta = String("");
}

void Arcana::init() {
    char* ssid = "Arcana";
    char* password = "2.718281";
    WiFi.mode(WIFI_STA); // Modo cliente WiFi
    WiFi.begin(ssid, password);
    pinMode(LED_BUILTIN, OUTPUT);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        digitalWrite(LED_BUILTIN, HIGH);
    }
    leerConfiguracion();
}


void Arcana::conectar() {
    leerRespuesta();
//    while (respuesta.substring(1, 4).equals("off")) {
//        Serial.println("Juego OFF. Esperando 2 segundos");
//        delay(2000);
//        leerRespuesta();
//    }
}

void Arcana::configurar() {
    leerConfiguracion();
}

void Arcana::leerConfiguracion() {
    char* host = "192.168.2.83";
    if (_useLED)  {
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
        digitalWrite(LED_BUILTIN, HIGH);
    }
    WiFiClient client;
    if (!client.connect(host, 80)) {
        Serial.println("No hay conexion");
        return;
    }
    String url = "/gizmo/config/"+_gizmoName;
    Serial.print(host);
    Serial.println(url);
    client.setNoDelay(1);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println("Timeout");
            client.stop();
            return;
        }
    }
    while (client.available()) {
        //        Serial.println("Client available");
        config = client.readStringUntil('\r');
    }
    client.stop();

    config = config.substring(1);
    Serial.println(config);
}


void Arcana::leerRespuesta() {
    char* host = "192.168.2.83";
    if (_useLED)  {
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
        digitalWrite(LED_BUILTIN, HIGH);
    }
    WiFiClient client;
    if (!client.connect(host, 80)) {
        Serial.println("No hay conexion");
        return;
    }
    String url = (_isBin) ? "/gizmo/reportbin/" : "/gizmo/report/";
    url += _gizmoName;
    url += "/?";
    url += String(_estado);
    Serial.print(host);
    Serial.println(url);
    client.setNoDelay(1);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println("Timeout");
            client.stop();
            return;
        }
    }
    while (client.available()) {
//        Serial.print(respuesta);
        respuesta = client.readStringUntil('\r');
    }
    client.stop();

    Serial.println(respuesta);
}

void Arcana::estado(String texto)
{
//    Serial.println("Nuevo estado:" + texto);
    _estado = texto;
}
