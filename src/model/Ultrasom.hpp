#include <Arduino.h>

class Ultrasom
{

private:
    int trigger, echo;
    float minDistance = 0.3;

public:
    void begin(int trigger, int echo)
    {
        this->trigger = trigger;
        this->echo = echo;
    }

    void begin(int trigger, int echo, float minDistance)
    {
        this->trigger = trigger;
        this->echo = echo;
        this->minDistance = minDistance;
    }

    long collectUltrasonicData()
    {
        pinMode(trigger, OUTPUT);
        digitalWrite(trigger, LOW);
        delay(2);
        digitalWrite(trigger, HIGH);
        delay(10);
        digitalWrite(trigger, LOW);
        pinMode(echo, INPUT);

        long pulse = pulseIn(echo, HIGH) / 100;

        if (pulse > minDistance) {
            return pulse;
        }
        return minDistance;
    }

    void setTrigger(int trigger)
    {
        this->trigger = trigger;
    }

    void setEcho(int echo)
    {
        this->echo = echo;
    }
};