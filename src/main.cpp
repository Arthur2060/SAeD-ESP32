#include "model/Color.hpp"

Color color;

void setup() {
  Serial.begin(9600);
  color.begin();
}

void loop() {
  std::vector<uint> valores = color.readColor();

  Serial.printf("R=%i, G=%i, B=%i, Luminosidade=%i", valores[0], valores[1], valores[2], valores[3]);
  delay(100);
}