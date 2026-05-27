#include <Arduino.h>

class Ultrasom
{

private:
    int trigger, echo;
    const int PULSE_PER_METER = 1.723;

public:
    void begin(int trigger, int echo)
    {
        this->trigger = trigger;
        this->echo = echo;
    }

    void begin(int trigger, int echo)
    {
        this->trigger = trigger;
        this->echo = echo;
    }

    double collectUltrasonicData()
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

    void setTrigger(int trigger)
    {
        this->trigger = trigger;
    }

    void setEcho(int echo)
    {
        this->echo = echo;
    }
};