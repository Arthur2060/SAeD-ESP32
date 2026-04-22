class Ultrasom
{

private:
    int trigger, echo;
    const float MIN_DISTANCE = 0.4;

public:
    void begin(int trigger, int echo)
    {
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

        long pulse = pulseIn(echo, HIGH) / 100;

        if (pulse > MIN_DISTANCE) {
            return pulse;
        }
        return 0;
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