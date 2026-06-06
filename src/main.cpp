#include <Arduino.h>
#include "view/Principal.h"

using namespace SAeD;

Principal prince(0.3);

void setup()
{
    prince.begin();
}

void loop()
{
    prince.principalLoop();
}