#include "headers/Ultrasom.h"

using namespace N;
using namespace std;

void Ultrasom::begin(int trigger, int echo)
{
    this->trigger = trigger;
    this->echo = echo;
}

double Ultrasom::collectUltrasonicData()
{
    pinMode(trigger, OUTPUT);
    digitalWrite(trigger, LOW);
    delay(2);
    digitalWrite(trigger, HIGH);
    delay(10);
    digitalWrite(trigger, LOW);
    pinMode(echo, INPUT);

    double distance = (PULSE_PER_METER * pulseIn(echo, HIGH));

    return distance;
}