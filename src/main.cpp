#include <Arduino.h>

// #include "TFLuna.hpp"
// #include "Bussola.hpp"
// #include "MicroRos.hpp"
#include "Ultrasom.hpp"

// TFLuna tfluna;
// Bussola bussola;
// MicroRos ros;
Ultrasom ultrasom(14, 12); // Pino 14 - Trigger, Pino 12 - Echo

void setup() {
    Serial.begin(9600);
}

void loop() {
    int distance = ultrasom.collectUltrasonicData();
    
    Serial.printf("Distance: %i\n", distance);
}