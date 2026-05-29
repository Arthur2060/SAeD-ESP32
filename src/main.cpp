#include <Arduino.h>
#include <Wire.h>
#include "model/Bussola.h"

Bussola bussola;

void setup()
{
  Serial.begin(9600);
  bussola.begin();
}

void loop()
{
  Serial.println(bussola.collectCompassData());
}