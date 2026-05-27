#include <Arduino.h>
#include <ESP32Encoder.h>
#include <vector>
#include <string>

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
    const float TRACK_WIDTH = 0.225f;       // metros (~20cm entre as rodas)

    int currentSpeed = 100; // %
    int currentDegrees = 0;

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
        // Pulsos por revolução do motor no eixo de saída: ENCODER_PPR * GEAR_RATIO * ENCODER_MULTIPLIER
        const float pulsesPerMeter = (ENCODER_PPR * ENCODER_MULTIPLIER * GEAR_RATIO) / (PI * WHEEL_DIAMETER);
        const long targetPulses = (long)(distance * pulsesPerMeter);

        encoderLeft.setCount(0);
        encoderRight.setCount(0);

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

        int accumulatorLeft = pwmValue;
        int accumulatorRight = pwmValue;

        delay(1000);
        while (abs(encoderLeft.getCount()) < targetPulses || abs(encoderRight.getCount()) < targetPulses)
        {
            if (encoderLeft.getCount() > encoderRight.getCount() + 2 && accumulatorRight < MAX_PWM && accumulatorLeft > MIN_PWM)
            {
                accumulatorRight += 5;
                accumulatorLeft -= 5;
                ledcWrite(right.PWM_PIN, accumulatorRight);
                ledcWrite(left.PWM_PIN, accumulatorLeft);
            }
            else if (encoderRight.getCount() > encoderLeft.getCount() + 2 && accumulatorLeft < MAX_PWM && accumulatorRight > MIN_PWM)
            {
                accumulatorRight -= 5;
                accumulatorLeft += 5;
                ledcWrite(right.PWM_PIN, accumulatorRight);
                ledcWrite(left.PWM_PIN, accumulatorLeft);
            }

            delay(10);
        }

        delay(1000);

        stopMotors();
    }

    void rotate(int degrees)
    {
        const float arcDistance = (TRACK_WIDTH * PI * abs(degrees)) / 360.0f;
        const float pulsesPerMeter = (ENCODER_PPR * ENCODER_MULTIPLIER * GEAR_RATIO) / (PI * WHEEL_DIAMETER);
        const long targetPulses = (long)(arcDistance * pulsesPerMeter);

        encoderLeft.setCount(0);
        encoderRight.setCount(0);

        if (degrees > 0)
        {
            // Giro para a esquerda: roda esquerda para trás, roda direita para frente
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, false);
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, true);

            int sum = currentDegrees + degrees;

            if (sum > 360) {
                int dif = 360 - currentDegrees;
                sum -= dif;
                currentDegrees = sum;
            }
        }
        else
        {
            // Giro para a direita: roda esquerda para frente, roda direita para trás
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, true);
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, false);

            int sub = currentDegrees - degrees;

            if (sub < 0) {
                currentDegrees = abs(sub);
            }
        }

        int pwmLeft = map(currentSpeed, 0, 100, MIN_PWM, MAX_PWM);
        int pwmRight = pwmLeft;
        ledcWrite(left.PWM_PIN, pwmLeft);
        ledcWrite(right.PWM_PIN, pwmRight);

        unsigned long startMillis = millis();
        const unsigned long timeout = 5000;

        while (abs(encoderLeft.getCount()) < targetPulses || abs(encoderRight.getCount()) < targetPulses)
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

        left.ENC_PIN_1 = 2;
        left.ENC_PIN_2 = 4;

        right.PWM_PIN = 0;
        right.IN1_PIN = 16;
        right.IN2_PIN = 17;

        right.ENC_PIN_1 = 32;
        right.ENC_PIN_2 = 33;
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

        stopMotors();
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