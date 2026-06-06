#ifndef CLAW_H
#define CLAW_H

#include <Arduino.h>

using namespace std;

namespace SAeD
{
    class Claw {
        public:
            Claw() {}

            void begin() {
                Serial.begin(115200);
            }

            void home() {
                Serial1.print(0);
            }
            void get() {
                Serial1.print(1);
            }
            void put() {
                Serial1.print(2);
            }
    };
}

#endif