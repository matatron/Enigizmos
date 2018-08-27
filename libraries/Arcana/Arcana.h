#ifndef Arcana_h
#define Arcana_h

#include "Arduino.h"
#include <ESP8266WiFi.h>

class Arcana
{
  public:
    Arcana(String gizmoName, bool LED = true, bool useBin = false);
    void init();
    void conectar();
    void configurar();
    void estado(String texto);
    String respuesta;
    String config;
  private:
    void leerRespuesta();
    void leerConfiguracion();
    String _gizmoName;
    bool _useLED;
    bool _isBin;
    String _estado;
};

#endif
