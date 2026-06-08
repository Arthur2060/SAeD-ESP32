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

        void idle();
        void get();
        void put();
    };
}

#endif
