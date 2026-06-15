#include "Motores.h"

using namespace SAeD;

void Motores::moveDistance(bool front)
{
    if (front)
    {
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, true);
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, true);
    }
    else
    {
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, false);
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, false);
    }
    ledcWrite(left.PWM_PIN, 255);
    ledcWrite(right.PWM_PIN, 255);

    delay(1350); // PELO AMOR DE DEUS, NÃO MUDE ISSO, NÃO SEI PORQUE, MAS QUALQUER OUTRA COISA NÃO SERVE!

    stopMotors();
}

void Motores::rotate(bool direction)
{
    float target;
    if (direction)
    {
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, false);
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, true);
    }
    else
    {
        // Giro para a direita: roda esquerda para frente, roda direita para trás
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, true);
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, false);
    }

    ledcWrite(left.PWM_PIN, 255);
    ledcWrite(right.PWM_PIN, 255);

    delay(2000);

    stopMotors();
}

void Motores::setMotorDirection(int in1, int in2, bool direction)
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

void Motores::stopMotors()
{
    ledcWrite(left.PWM_PIN, 0);
    ledcWrite(right.PWM_PIN, 0);
}

Motores::Motores()
{
    left.PWM_PIN = 15;
    left.IN1_PIN = 2;
    left.IN2_PIN = 4;

    left.ENC_PIN_1 = 25;
    left.ENC_PIN_2 = 26;

    right.PWM_PIN = 0;
    right.IN1_PIN = 16;
    right.IN2_PIN = 17;

    right.ENC_PIN_1 = 32;
    right.ENC_PIN_2 = 33;
}

void Motores::begin()
{
    ledcSetup(left.PWM_PIN, 1000, 8);
    ledcAttachPin(left.PWM_PIN, left.PWM_PIN);
    pinMode(left.IN1_PIN, OUTPUT);
    pinMode(left.IN2_PIN, OUTPUT);

    ledcSetup(right.PWM_PIN, 1000, 8);
    ledcAttachPin(right.PWM_PIN, right.PWM_PIN);
    pinMode(right.IN1_PIN, OUTPUT);
    pinMode(right.IN2_PIN, OUTPUT);

    stopMotors();
}

bool Motores::lerComandos(char *commands)
{
    for (int c = 0; c < sizeof(commands) / sizeof(commands[0]); c++)
    {
        switch (commands[c])
        {
        case 'W':
            moveDistance(true);
            break;
        case 'S':
            moveDistance(false);
            break;
        case 'A':
            rotate(true);
            break;
        case 'D':
            rotate(false);
            break;
        case 'R':
            rotate(true);
            rotate(true);
            break;
        default:
            Serial.println("Comando inválido!");
            return false;
        }
    }
    delete[] commands;

    return true;
}

void Motores::setSpeed(int newSpeed)
{
    if (newSpeed >= 0 && newSpeed <= 100)
    {
        currentSpeed = newSpeed;
    }
}