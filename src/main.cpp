#include <Arduino.h>
#include "controll/Motores.h"

Motores motores;

void setup()
{
  Serial.begin(9600);
  motores.begin();
}

void loop()
{
  motores.lerComandos({'W', 'A', 'S', 'D'});
}