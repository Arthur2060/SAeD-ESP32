#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <vector>
#include <string>

namespace N
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

        // Encoders
        ESP32Encoder encoderLeft;
        ESP32Encoder encoderRight;

        // Constantes do Sistema
        const int MAX_RPM = 170;
        const int MAX_PWM = 255;
        const int MIN_PWM = 50;                // Mínimo diferente de zero
        const int ENCODER_PPR = 47;            // Pulsos por revolução do encoder
        const float ENCODER_MULTIPLIER = 2.0f; // attachHalfQuad conta duas transições por pulso
        const int CELL_DISTANCE = 0.3;         // metros (distância de uma célula)
        const int GEAR_RATIO = 35;             // Razão de redução típica
        const float WHEEL_DIAMETER = 0.07f;    // metros (~7cm)
        const float TRACK_WIDTH = 0.225f;      // metros (~20cm entre as rodas)

        int currentSpeed = 100; // %
        int currentDegrees = 0;

        void moveForward();
        void moveBackward();
        void turnLeft();
        void turnRight();
        void spin360();

        void moveDistance(float distance, bool direction);
        void rotate(int degrees);
        void setMotorDirection(int in1, int in2, bool direction);
        void stopMotors();

    public:
        Motores();
        void begin();
        bool lerComandos(std::vector<char> commands);
        void setSpeed(int newSpeed);
    };

}

#endif