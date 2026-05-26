#include <Arduino.h>
#include <vector>
#include <string>
#include "view/SAeD.h"

SAeD saed;

std::vector<char> testCommands = {'W', 'S', 'D', 'A'};

void setup()
{
    Serial.begin(9600);
    saed.begin();
}

void loop()
{
    saed.received();
}