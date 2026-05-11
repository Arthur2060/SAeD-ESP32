#include <Arduino.h>
#include <vector>
#include "view/WebIHM.hpp"

#define SSID "Arthur"
#define PASSWORD "Arthur1705"

WebIHM ihm;

void setup() {
    Serial.begin(9600);
    ihm.begin(SSID, PASSWORD);
}

void loop() {}