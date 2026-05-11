#include <Arduino.h>
#include <vector>
#include "controll/Radar.hpp"

Radar radar;

void setup() {
    Serial.begin(9600);
    radar.begin();
}

void loop() {

    std::vector<float> obstacle = radar.getRawSensors();

    Serial.printf("Distance=%.2f, Angles { X=%.2f, Y=%.2f, Z=%.2f }\n", obstacle[0], obstacle[1], obstacle[2], obstacle[3]);
    delay(300);
}