#ifndef ULTRASOM_H
#define ULTRASOM_H

#include <Arduino.h>

namespace N
{
    class Ultrasom
    {

    private:
        int trigger, echo;
        const int PULSE_PER_METER = 1.723;

    public:
        void begin(int trigger, int echo);

        double collectUltrasonicData();
    };
}

#endif