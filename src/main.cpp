#include <Arduino.h>
#include "view/Principal.h"

using namespace SAeD;

Principal prince(10, 10, 0.3);

TaskHandle_t WatchDog = NULL;

void loop2(void *parameter);

void setup()
{
    prince.begin();

    xTaskCreatePinnedToCore(
        loop2,     // Task function
        "loop2",   // Task name
        10000,     // Stack size (bytes)
        NULL,      // Parameters
        1,         // Priority
        &WatchDog, // Task handle
        1          // Core 1
    );
}

void loop()
{
}

void loop2(void* parameter) {
    for (;;) {
        prince.secondaryLoop();
    }
}