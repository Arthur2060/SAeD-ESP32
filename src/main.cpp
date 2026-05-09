#include <Arduino.h>
#include "controll/Motores.hpp"

Motores mot;

TaskHandle_t interno;

void loop2( void * pvParameters);

void setup() {
    Serial.begin(9600);

    mot.begin();

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

    mot.lerComandos("WWWS");
    delay(1000);
}

void loop2( void * pvParameters) {
    Serial.printf("Iniciando processamento em Core %i", xPortGetCoreID);
    for(;;) {
    }
}
