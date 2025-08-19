#ifndef Arcana3_h
#define Arcana3_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

class Arcana3
{
  public:
    Arcana3(String gizmoName, bool LED = true);
    void init();
    void conectar();
    void configurar();
    bool acertijo(int n);
    void estado(String texto);
    int status;
    StaticJsonDocument<300> config;
    StaticJsonDocument<300> data;
  private:
    void leerRespuesta();
    void leerConfiguracion();
    String _gizmoName;
    bool _useLED;
    bool _isBin;
    String _estado;
};

#endif
