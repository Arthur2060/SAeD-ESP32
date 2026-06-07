#include <Arduino.h>
#include "Principal.h"

using namespace SAeD;

Principal prince(0.3);

void setup()
{
    Serial.begin(9600);
    prince.begin();
}

void loop()
{
    prince.principalLoop();
}