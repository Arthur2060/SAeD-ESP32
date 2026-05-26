#include <Arduino.h>
#include <vector>
#include <string>
#include "model/Bussola.h"

Bussola bussola;

std::vector<char> testCommands = {'W', 'S', 'D', 'A'};

void setup()
{
    Serial.begin(9600);
    bussola.begin();
}

void loop()
{
    Serial.printf("angle=%.2f\n", bussola.collectCompassData());
}