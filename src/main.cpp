#include <Arduino.h>

#include "TFLuna.hpp"
#include "Bussola.hpp"
#include "MicroRos.hpp"
#include "Ultrasom.hpp"

TFLuna tfluna;
Bussola bussola;
MicroRos ros;
Ultrasom ultrasom(7, 7);

void setup() {
    Serial.begin(115900);
}

void loop() {
    float distance = ultrasom.collectUltrasonicData();
    float angle = bussola.collectCompassData();
    ros.sendData(distance, angle);
}