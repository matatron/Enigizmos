#include <Arduino.h>
#include "Arcana3.h"

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

WiFiClient client;
HTTPClient http;

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

Arcana3::Arcana3(String gizmoName, bool LED)
{
    _gizmoName = gizmoName;
    _useLED = LED;
    _estado = "";
    status = 0;
}

void Arcana3::init() {
    WiFi.mode(WIFI_STA);
    WiFi.begin("Enigmata", "2.718281");

    pinMode(LED_BUILTIN, OUTPUT);

    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print('.');
    }
    leerConfiguracion();
}


void Arcana3::conectar() {
    leerRespuesta();
}

void Arcana3::configurar() {
    leerConfiguracion();
}

bool Arcana3::acertijo(int n) {
    bool response = false;
    String progress = (String) data["progress"];
    if (progress) {
        response = progress.charAt(n-1) == '1';
    }
    return response;
}


void Arcana3::leerConfiguracion() {
    if (http.begin(client, "http://192.168.2.83/gizmo/config/"+_gizmoName)) {  // HTTP
        if (_useLED)  {
            digitalWrite(LED_BUILTIN, LOW);
            delay(50);
            digitalWrite(LED_BUILTIN, HIGH);
        }

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = http.getString();
                Serial.println(payload);
                DeserializationError error = deserializeJson(config, payload);
                if (error) {
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(error.c_str());
                    return;
                }
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
    }
}


void Arcana3::leerRespuesta() {
    if (http.begin(client, "http://192.168.2.83/gizmo/report/"+_gizmoName+"?"+String(_estado))) {  // HTTP
        if (_useLED)  {
            digitalWrite(LED_BUILTIN, LOW);
            delay(50);
            digitalWrite(LED_BUILTIN, HIGH);
        }

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = http.getString();
                DeserializationError error = deserializeJson(data, payload);
                if (error) {
                    Serial.print(("deserializeJson() failed: "));
                    Serial.println(error.c_str());
                    return;
                }
                status = (int) data["status"];
                //Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
    }
    
}

void Arcana3::estado(String texto)
{
//    Serial.println("Nuevo estado:" + texto);
    //texto.replace(' ', '_');
    texto.replace(" ", "%20");
    _estado = texto;
}
