#include <Arduino.h>
#include <vector>
#include <string>
#include "controll/Motores.h"

Motores motores;

std::vector<char> testCommands = {'W', 'S', 'D', 'A'};

void setup()
{
    Serial.begin(9600);
    motores.begin();
}

void loop()
{
    Serial.println("Girando");

    delay(2000);

    motores.lerComandos({
        'W',
        'D',
        'W',
        'D',
        'W',
        'D',
        'W',
        'D',
    });
}