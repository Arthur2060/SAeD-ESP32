#include <Arduino.h>

#include "TFLuna.hpp"
#include "Bussola.hpp"
#include "MicroRos.hpp"

TFLuna tfluna;
Bussola bussola;
MicroRos ros;

void setup() {
    Serial.begin(115900);
}

void loop() {
    float distance = tfluna.collectDataUART();
    float angle = bussola.collectCompassData();
    ros.sendData(distance, angle);
}