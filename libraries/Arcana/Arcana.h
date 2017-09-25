#ifndef Arcana_h
#define Arcana_h

#include "Arduino.h"
#include <ESP8266WiFi.h>

class Arcana
{
  public:
    Arcana(String gizmoName, bool LED);
    void init();
    void conectar();
    void estado(String texto);
    String respuesta;
  private:
    void leerRespuesta();
    String _gizmoName;
    bool _useLED;
    String _estado;
};

#endif
