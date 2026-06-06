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
                Serial1.write(0);
            }
    };
}

#endif