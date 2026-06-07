#ifndef MOTORES_H
#define MOTORES_H

#include "Bussola.h"

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <vector>
#include <map>
#include <string>

namespace SAeD
{
    enum class possibleAngles
    {
        ZERO,
        NOVENTA,
        CENTO_E_OITENTA,
        MENOS_NOVENTA
    };

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

        Bussola bussola;

        // Encoders
        ESP32Encoder encoderLeft;
        ESP32Encoder encoderRight;

        std::map<float, possibleAngles> selectCurrentAngle = {
            {0, possibleAngles::ZERO},
            {90, possibleAngles::NOVENTA},
            {180, possibleAngles::CENTO_E_OITENTA},
            {-90, possibleAngles::MENOS_NOVENTA}};

        std::map<possibleAngles, float> selectEquivalentAngle = {
            {possibleAngles::ZERO, 0},
            {possibleAngles::NOVENTA, 90},
            {possibleAngles::CENTO_E_OITENTA, 180},
            {possibleAngles::MENOS_NOVENTA, -90}};

        std::map<possibleAngles, possibleAngles> selectNextAngle = {
            {possibleAngles::ZERO, possibleAngles::NOVENTA},
            {possibleAngles::NOVENTA, possibleAngles::CENTO_E_OITENTA},
            {possibleAngles::CENTO_E_OITENTA, possibleAngles::MENOS_NOVENTA},
            {possibleAngles::MENOS_NOVENTA, possibleAngles::ZERO}};

        std::map<possibleAngles, possibleAngles> selectPreviousAngle = {
            {possibleAngles::ZERO, possibleAngles::MENOS_NOVENTA},
            {possibleAngles::MENOS_NOVENTA, possibleAngles::CENTO_E_OITENTA},
            {possibleAngles::CENTO_E_OITENTA, possibleAngles::NOVENTA},
            {possibleAngles::NOVENTA, possibleAngles::ZERO}};

        // Constantes do Sistema
        const int MAX_RPM = 170;
        const int MAX_PWM = 255;
        const int MIN_PWM = 50;                // Mínimo diferente de zero
        const int ENCODER_PPR = 47;            // Pulsos por revolução do encoder
        const float ENCODER_MULTIPLIER = 2.0f; // attachHalfQuad conta duas transições por pulso
        int cellScale = 0.3;                   // metros (distância de uma célula)
        const int GEAR_RATIO = 35;             // Razão de redução típica
        const float WHEEL_DIAMETER = 0.07f;    // metros (~7cm)
        const float TRACK_WIDTH = 0.225f;      // metros (~20cm entre as rodas)
        const int ONE_METER_DELAY = 4500;

        // Pulsos por revolução do motor no eixo de saída: ENCODER_PPR * GEAR_RATIO * ENCODER_MULTIPLIER
        const float PULSE_PER_METER = (ENCODER_PPR * ENCODER_MULTIPLIER * GEAR_RATIO);
        const long TARGET_PULSE = (long)(cellScale * PULSE_PER_METER);

        int currentSpeed = 100; // %
        possibleAngles currentDegrees = possibleAngles::ZERO;

        void moveForward();
        void moveBackward();
        void turnLeft();
        void turnRight();
        void spin360();

        void moveDistance(bool direction);
        void rotate(bool direction);
        void setMotorDirection(int in1, int in2, bool direction);
        void stopMotors();
        void correctOrientation();

    public:
        Motores();
        void setCellScale(float cellScale);
        void begin();
        bool lerComandos(std::vector<char> comandos);
        void setSpeed(int newSpeed);
    };
}

#endif
