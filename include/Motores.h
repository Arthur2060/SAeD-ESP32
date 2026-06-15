#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>

namespace SAeD
{
    struct Motor
    {
        int PWM_PIN;
        int IN1_PIN;
        int IN2_PIN;

        int ENC_PIN_1;
        int ENC_PIN_2;
    };

    class Motores
    {
    private:
        Motor left, right;

        int currentSpeed = 100; // %

        void moveForward();
        void moveBackward();
        void turnLeft();
        void turnRight();
        void spin360();

        void moveDistance(bool front);
        void rotate(bool direction);
        void setMotorDirection(int in1, int in2, bool direction);
        void stopMotors();
        void correctOrientation();

    public:
        Motores();
        void setCellScale(float cellScale);
        void begin();
        bool lerComandos(char* comandos);
        void setSpeed(int newSpeed);
    };
}

#endif
