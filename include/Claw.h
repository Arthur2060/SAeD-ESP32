#ifndef CLAW_H
#define CLAW_H

#include <Arduino.h>

using namespace std;

namespace SAeD
{
    class Claw
    {
    public:
        Claw() {}

        void home()
        {
            Serial.println(0);
        }
        void get()
        {
            Serial.println(1);
        }
        void put()
        {
            Serial.println(2);
        }
    };
}

#endif
