#include <Arduino.h>
#include "view/SAeD.hpp"

TaskHandle_t Interno;

SAeD saed;


void setup() {
    Serial.begin(9600);
    saed.begin();

    Serial.printf("Iniciando processamento em Core %i", xPortGetCoreID);
}

void loop() {
    xTaskCreatePinnedToCore(
        loop2, /* Function to implement the task */
        "Task1",   /* Name of the task */
        10000,     /* Stack size in words */
        NULL,      /* Task input parameter */
        0,         /* Priority of the task */
        &Interno,    /* Task handle. */
        0);        /* Core where the task should run */

    saed.principalLoop();
}

void loop2( void * pvParameters) {
    Serial.printf("Iniciando processamento em Core %i", xPortGetCoreID);
    for(;;) {
        saed.secondaryLoop();
    }
}
