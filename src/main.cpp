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

    motores.setSpeed(100);
}

void loop()
{
    Serial.print("Testando motores, movimento de teste: ");
    Serial.print(testCommands[0]);
    Serial.println(" em...");
    Serial.println("3...");
    delay(1000);
    Serial.println("2..");
    delay(1000);
    Serial.println("1!");
    delay(1000);

    if (motores.lerComandos(testCommands))
    {
        Serial.println("Teste concluido com exito!");
    }
    else
    {
        Serial.println("Falha ao executar teste...");
    }
}