#include <Arduino.h>
#include <vector>
#include "controll/Motores.hpp"
#include "controll/Radar.hpp"

Motores mot;
Radar radar;

TaskHandle_t interno;

void loop2( void * pvParameters);

void setup() {
    Serial.begin(9600);

    mot.begin();
    radar.begin();

    Serial.printf("Iniciando processamento em Core %i", xPortGetCoreID);
}

void loop() {
    xTaskCreatePinnedToCore(
        loop2,      /* Function to implement the task */
        "Task1",    /* Name of the task               */
        10000,      /* Stack size in words            */
        NULL,       /* Task input parameter           */
        0,          /* Priority of the task           */
        &interno,   /* Task handle.                   */
        0);         /* Core where the task should run */

    char * commands = "R";
    mot.lerComandos(commands);
    delay(1000);
}

void loop2( void * pvParameters) {
    Serial.printf("Iniciando processamento em Core %i", xPortGetCoreID);
    for(;;) {
        std::vector<float> obstacle = radar.getObstacle();

        Serial.printf("X=%.2f, Y=%.2f", obstacle[0], obstacle[1]);
    }
}
