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
    rotate(90);
}

void Motores::turnRight()
{
    rotate(-90);
}

void Motores::spin360()
{
    rotate(360);
}

void Motores::moveDistance(bool direction)
{
    if (direction)
    {
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, true); // Frente
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, true);
    }
    else
    {
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, false); // Trás
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, false);
    }

    const int pwmValue = map(currentSpeed, 0, 100, MIN_PWM, MAX_PWM);
    ledcWrite(left.PWM_PIN, pwmValue);
    ledcWrite(right.PWM_PIN, pwmValue);

    delay(1350); // PELO AMOR DE DEUS, NÃO MUDE ISSO, NÃO SEI PORQUE, MAS QUALQUER OUTRA COISA NÃO SERVE!

    stopMotors();
}

void Motores::rotate(int degrees)
{
    const float arcDistance = (TRACK_WIDTH * PI * abs(degrees)) / 360.0f;
    const float pulsesPerMeter = (ENCODER_PPR * ENCODER_MULTIPLIER * GEAR_RATIO) / (PI * WHEEL_DIAMETER);

    encoderLeft.setCount(0);
    encoderRight.setCount(0);

    if (degrees > 0)
    {
        // Giro para a esquerda: roda esquerda para trás, roda direita para frente
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, false);
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, true);
    }
    else
    {
        // Giro para a direita: roda esquerda para frente, roda direita para trás
        setMotorDirection(left.IN1_PIN, left.IN2_PIN, true);
        setMotorDirection(right.IN1_PIN, right.IN2_PIN, false);
    }

    int pwmLeft = map(currentSpeed, 0, 100, MIN_PWM, MAX_PWM);
    int pwmRight = pwmLeft;
    ledcWrite(left.PWM_PIN, pwmLeft);
    ledcWrite(right.PWM_PIN, pwmRight);

    unsigned long startMillis = millis();
    const unsigned long timeout = 5000;

    while (abs(encoderLeft.getCount()) < TARGET_PULSE || abs(encoderRight.getCount()) < TARGET_PULSE)
    {
        if (millis() - startMillis > timeout)
        {
            break;
        }

        if (abs(encoderLeft.getCount()) > abs(encoderRight.getCount()) + 2 && pwmRight < MAX_PWM && pwmLeft > MIN_PWM)
        {
            pwmRight += 5;
            pwmLeft -= 5;
            ledcWrite(right.PWM_PIN, pwmRight);
            ledcWrite(left.PWM_PIN, pwmLeft);
        }
        else if (abs(encoderRight.getCount()) > abs(encoderLeft.getCount()) + 2 && pwmLeft < MAX_PWM && pwmRight > MIN_PWM)
        {
            pwmRight -= 5;
            pwmLeft += 5;
            ledcWrite(right.PWM_PIN, pwmRight);
            ledcWrite(left.PWM_PIN, pwmLeft);
        }

        delay(10);
    }

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

    encoderLeft.attachHalfQuad(left.ENC_PIN_1, left.ENC_PIN_2);
    encoderRight.attachHalfQuad(right.ENC_PIN_1, right.ENC_PIN_2);

    encoderLeft.setCount(0);
    encoderRight.setCount(0);

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

void Motores::setCellScale(float cellScale) {
    this->cellScale = cellScale;
}