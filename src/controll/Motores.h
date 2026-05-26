#include <Arduino.h>
#include <ESP32Encoder.h>
#include <vector>
#include <string>
#include "Radar.hpp"

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
    Radar radar;

    // Encoders
    ESP32Encoder encoderLeft;
    ESP32Encoder encoderRight;

    // Constantes do Sistema
    const int MAX_RPM = 170;
    const int MAX_PWM = 255;
    const int MIN_PWM = 50;        // Mínimo diferente de zero
    const int ENCODER_PPR = 11;    // Pulsos por revolução do motor
    const int CELL_DISTANCE = 0.3; // metros (distância de uma célula)
    const int GEAR_RATIO = 35;     // Razão de redução típica

    int currentSpeed = 100; // %
    int currentDegrees = 0; // %

    void moveForward()
    {
        moveDistance(CELL_DISTANCE, true);
    }

    void moveBackward()
    {
        moveDistance(CELL_DISTANCE, false);
    }

    void turnLeft()
    {
        rotate(90);
    }

    void turnRight()
    {
        rotate(-90);
    }

    void spin360()
    {
        rotate(360);
    }

    void moveDistance(float distance, bool direction)
    {
        // Calcular pulsos necessários
        // Pulsos por revolução (com gear): ENCODER_PPR * GEAR_RATIO
        // Distância por pulso: π * D_RODA / (ENCODER_PPR * GEAR_RATIO)

        float pulsesPerMeter = (ENCODER_PPR * GEAR_RATIO) / (3.14159 * 0.07); // D_RODA ~= 7cm
        long targetPulses = (long)(distance * pulsesPerMeter);

        encoderLeft.setCount(0);
        encoderRight.setCount(0);

        if (direction)
        {
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, 1); // Frente
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, 1);
        }
        else
        {
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, 0); // Trás
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, 0);
        }

        int pwmValue = map(currentSpeed, 0, 100, MIN_PWM, MAX_PWM);

        ledcWrite(left.PWM_PIN, pwmValue);
        ledcWrite(right.PWM_PIN, pwmValue);

        int accumulatorLeft = pwmValue;
        int accumulatorRight = pwmValue;

        delay(1000);
        while (abs(encoderLeft.getCount()) < targetPulses && abs(encoderRight.getCount()) < targetPulses)
        {
            if (encoderLeft.getCount() > encoderRight.getCount() + 2 && accumulatorRight > 5 && accumulatorLeft < 100)
            {
                ledcWrite(right.PWM_PIN, accumulatorRight += 5);
                ledcWrite(left.PWM_PIN, accumulatorLeft -= 5);
            }
            else if (encoderRight.getCount() > encoderLeft.getCount() + 2 && accumulatorRight > 5 && accumulatorLeft < 100)
            {
                ledcWrite(right.PWM_PIN, accumulatorRight -= 5);
                ledcWrite(left.PWM_PIN, accumulatorLeft += 5);
            }

            delay(10);
        }

        delay(1000);

        stopMotors();
    }

    void rotate(int degrees)
    {
        /*
        Diâmetro entre esteiras (distância entre trilhos)
        float wheelDistance = 0.20; // ~20cm - AJUSTE CONFORME SEU CARRO
        float arcDistance = (wheelDistance * 3.14159 * abs(degrees)) / 360.0;

        encoderLeft.setCount(0);
        encoderRight.setCount(0);

        float pulsesPerMeter = (ENCODER_PPR * GEAR_RATIO) / (3.14159 * 0.07);
        long targetPulses = (long)(arcDistance * pulsesPerMeter);
        */

        currentDegrees = degrees;

        if (currentDegrees > 0)
        {
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, 1);
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, 0);
        }
        else
        {
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, 0);
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, 1);
        }

        int pwmValue = map(currentSpeed, 0, 100, MIN_PWM, MAX_PWM);
        ledcWrite(left.PWM_PIN, pwmValue);
        ledcWrite(right.PWM_PIN, pwmValue);

        while (radar.getRawSensors()[1] != currentDegrees)
        {
            delay(10);
        }

        stopMotors();
    }

    void centralize()
    {
        currentDegrees = 0;

        if (currentDegrees > 0)
        {
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, 1);
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, 0);
        }
        else
        {
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, 0);
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, 1);
        }

        int pwmValue = map(currentSpeed, 0, 100, MIN_PWM, MAX_PWM);
        ledcWrite(left.PWM_PIN, pwmValue);
        ledcWrite(right.PWM_PIN, pwmValue);

        while (radar.getRawSensors()[1] != currentDegrees)
        {
            delay(10);
        }

        stopMotors();
    }

    void setMotorDirection(int in1, int in2, bool direction)
    {
        if (direction)
        {
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
        }
        else
        {
            digitalWrite(in1, LOW);
            digitalWrite(in2, HIGH);
        }
    }

    void stopMotors()
    {
        ledcWrite(left.PWM_PIN, 0);
        ledcWrite(right.PWM_PIN, 0);
    }

public:
    Motores()
    {
        left.PWM_PIN = 15;
        left.IN1_PIN = 2;
        left.IN2_PIN = 4;

        left.ENC_PIN_1 = 26;
        left.ENC_PIN_2 = 25;

        right.PWM_PIN = 13;
        right.IN1_PIN = 18;
        right.IN2_PIN = 19;

        right.ENC_PIN_1 = 33;
        right.ENC_PIN_2 = 32;
    }

    void begin()
    {
        ledcSetup(left.PWM_PIN, 1000, 8);
        ledcAttachPin(left.PWM_PIN, left.PWM_PIN);
        pinMode(left.IN1_PIN, OUTPUT);
        pinMode(left.IN2_PIN, OUTPUT);

        ledcSetup(right.PWM_PIN, 1000, 8);
        ledcAttachPin(right.PWM_PIN, right.PWM_PIN);
        pinMode(right.IN1_PIN, OUTPUT);
        pinMode(right.IN2_PIN, OUTPUT);

        encoderLeft.attachHalfQuad(left.ENC_PIN_1, left.ENC_PIN_2);
        encoderRight.attachHalfQuad(right.ENC_PIN_1, right.ENC_PIN_2);

        encoderLeft.setCount(0);
        encoderRight.setCount(0);

        centralize();
        stopMotors();
        radar.begin();
    }

    bool lerComandos(std::vector<char> commands)
    {
        for (int c = 0; c < commands.size(); c++)
        {
            switch (commands[c])
            {
            case 'W':
                moveForward();
                break;
            case 'S':
                moveBackward();
                break;
            case 'A':
                turnLeft();
                break;
            case 'D':
                turnRight();
                break;
            case 'R':
                spin360();
                break;
            default:
                Serial.println("Comando inválido!");
                return false;
            }
        }

        return true;
    }

    void setSpeed(int newSpeed)
    {
        if (newSpeed >= 0 && newSpeed <= 100)
        {
            currentSpeed = newSpeed;
        }
    }
};