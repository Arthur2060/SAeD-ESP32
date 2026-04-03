#include <Arduino.h>

class Ultrasom {
    public:

        Ultrasom(int trigger, int echo) {
            this->trigger = trigger;
            this->echo = echo;
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
            return pulseIn(echo, HIGH) / 100;
        }

        void setTrigger(int trigger)
        {
            this->trigger = trigger;
        }

        void setEcho(int echo)
        {
            this->echo = echo;
        }

    private:
        int trigger, echo;
        const MIN_DISTANCE = 0.4;
};