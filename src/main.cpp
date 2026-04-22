#include "controll/Radar.hpp"

Radar radar;

void setup() {
  Serial.begin(9600);
  radar.begin(14, 13);
}

void loop() {
  float leitura[2] = {radar.getObstacle()[0], radar.getObstacle()[1]};
  Serial.printf("X = %.2fm, Y = %.2fm\n", leitura[0], leitura[1]);
  delay(1000);
}