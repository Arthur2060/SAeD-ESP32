#include <Arduino.h>
#include <vector>
#include "controll/Radar.hpp"

Radar radar;

void setup() {
    Serial.begin(9600);
    radar.begin();
}

void loop() {

    std::vector<double> obstacle = radar.getRawSensors();

    Serial.printf("Distance=%.2f, Angles=%.2f\n", obstacle[0], obstacle[1]);
    delay(300);
}