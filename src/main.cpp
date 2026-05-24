#include <Arduino.h>
#include <vector>
#include "view/WebIHM.hpp"

char* SSID = "Arthur";
char* PASSWORD = "Arthur1705";

WebIHM ihm;

void setup() {
    Serial.begin(9600);
    Serial.println("Monitor serial iniciado...");
    ihm.begin(SSID, PASSWORD);
    Serial.println("Servido iniciado...");

    Serial.print("IP do servidor: ");
    Serial.println(ihm.getIp());
}

void loop() {}