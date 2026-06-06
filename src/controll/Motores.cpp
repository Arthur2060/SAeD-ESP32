#include "Motores.h"

using namespace SAeD;
using namespace std;

void Motores::moveForward()
{
    moveDistance(true);
}

void Motores::moveBackward()
{
    moveDistance(false);
}

void Motores::turnLeft()
{
    rotate(true);
}

void Motores::turnRight()
{
    rotate(false);
}

void Motores::spin360()
{
    rotate(true);
    rotate(true);
    rotate(true);
    rotate(true);
}

void Motores::moveDistance(bool direction)
{
    if (direction)
    {
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, false); // Trás
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, false);
    }
    else
    {
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, true);
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, true); // Frente
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
        // Giro para a esquerda: roda esquerda para trás, roda direita para frente
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, false);
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, true);

        target = selectEquivalentAngle[selectNextAngle[currentDegrees]];
    }
    else
    {
        // Giro para a direita: roda esquerda para frente, roda direita para trás
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, true);
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, false);

        target = selectEquivalentAngle[selectPreviousAngle[currentDegrees]];
    }

    ledcWrite(left.PWM_PIN, 255);
    ledcWrite(right.PWM_PIN, 255);

    float actualCompass = bussola.collectCompassData();

    while (abs(actualCompass - target) > 1)
    {
        actualCompass = bussola.collectCompassData();
        delay(1);
    }

    stopMotors();
    currentDegrees = selectCurrentAngle[target];
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

    encoderLeft.attachHalfQuad(left.ENC_PIN_1, left.ENC_PIN_2);
    encoderRight.attachHalfQuad(right.ENC_PIN_1, right.ENC_PIN_2);

    encoderLeft.setCount(0);
    encoderRight.setCount(0);

    setMotorDirection(left.IN1_PIN, left.IN2_PIN, false);
    setMotorDirection(right.IN1_PIN, right.IN2_PIN, true);

    correctOrientation();
    stopMotors();
}

bool Motores::lerComandos(std::vector<char> commands)
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
            turnRight();
            break;
        case 'D':
            turnLeft();
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

void Motores::setSpeed(int newSpeed)
{
    if (newSpeed >= 0 && newSpeed <= 100)
    {
        currentSpeed = newSpeed;
    }
}

void Motores::setCellScale(float cellScale)
{
    this->cellScale = cellScale;
}

void Motores::correctOrientation()
{
    ledcWrite(left.PWM_PIN, 255);
    ledcWrite(right.PWM_PIN, 255);

    while (bussola.collectCompassData() != selectEquivalentAngle[currentDegrees])
    {
        delay(10);
    }

    stopMotors();
}