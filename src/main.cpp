#include "controll/Radar.hpp"
#include "model/Motores.hpp"

Radar radar;
Motores motores;

void setup() {
    radar.begin();
    motores.begin();
}

void loop() {

    radar.getObstacle(motores.atualizarOdometria());
}