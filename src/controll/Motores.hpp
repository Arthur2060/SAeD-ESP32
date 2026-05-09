#include <Arduino.h>
#include <ESP32Encoder.h>
#include <string.h>

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
    const int MIN_PWM = 50;        // Mínimo diferente de zero
    const int ENCODER_PPR = 11;    // Pulsos por revolução do motor
    const int CELL_DISTANCE = 0.3; // metros (distância de uma célula)
    const int GEAR_RATIO = 35;     // Razão de redução típica

    int currentSpeed = 100; // Velocidade atual em %

    void moveForward()
    {
        moveDistance(CELL_DISTANCE, 1); // 1 = para frente
    }

    void moveBackward()
    {
        moveDistance(CELL_DISTANCE, -1); // -1 = para trás
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

    // ============================================================
    // CONTROLE DE MOVIMENTO LINEAR
    // ============================================================

    void moveDistance(float distance, int direction)
    {
        // Calcular pulsos necessários
        // Pulsos por revolução (com gear): ENCODER_PPR * GEAR_RATIO
        // Distância por pulso: π * D_RODA / (ENCODER_PPR * GEAR_RATIO)

        float pulsesPerMeter = (ENCODER_PPR * GEAR_RATIO) / (3.14159 * 0.07); // D_RODA ~= 7cm
        long targetPulses = (long)(distance * pulsesPerMeter);

        // Reset encoders
        encoderLeft.setCount(0);
        encoderRight.setCount(0);

        // Determinar direção
        if (direction > 0)
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
        analogWrite(left.PWM_PIN, pwmValue);
        analogWrite(right.PWM_PIN, pwmValue);

        // Aguardar até atingir distância
        while (abs(encoderLeft.getCount()) < targetPulses && abs(encoderRight.getCount()) < targetPulses)
        {
            // Sincronização em malha fechada
            if (encoderLeft.getCount() > encoderRight.getCount() + 2)
            {
                analogWrite(right.PWM_PIN, pwmValue + 5);
                analogWrite(left.PWM_PIN, pwmValue - 5);
            }
            else if (encoderRight.getCount() > encoderLeft.getCount() + 2)
            {
                analogWrite(right.PWM_PIN, pwmValue - 5);
                analogWrite(left.PWM_PIN, pwmValue + 5);
            }
            delay(10);
        }

        stopMotors();
    }

    // ============================================================
    // CONTROLE DE ROTAÇÃO
    // ============================================================

    void rotate(int degrees)
    {
        // Diâmetro entre esteiras (distância entre trilhos)
        float wheelDistance = 0.20; // ~20cm - AJUSTE CONFORME SEU CARRO
        float arcDistance = (wheelDistance * 3.14159 * abs(degrees)) / 360.0;

        // Reset encoders
        encoderLeft.setCount(0);
        encoderRight.setCount(0);

        float pulsesPerMeter = (ENCODER_PPR * GEAR_RATIO) / (3.14159 * 0.07);
        long targetPulses = (long)(arcDistance * pulsesPerMeter);

        if (degrees > 0)
        {
            // Horário: esquerda frente, direita trás
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, 1); // Frente
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, 0);
        }
        else
        {
            setMotorDirection(left.IN1_PIN, left.IN2_PIN, 0); // Trás
            setMotorDirection(right.IN1_PIN, right.IN2_PIN, 1);
        }

        int pwmValue = map(currentSpeed, 0, 100, MIN_PWM, MAX_PWM);
        analogWrite(left.PWM_PIN, pwmValue);
        analogWrite(right.PWM_PIN, pwmValue);

        while (abs(encoderLeft.getCount()) < targetPulses || abs(encoderRight.getCount()) < targetPulses)
        {
            delay(10);
        }

        stopMotors();
    }

    // ============================================================
    // FUNÇÕES AUXILIARES
    // ============================================================

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
        analogWrite(left.PWM_PIN, 0);
        analogWrite(right.PWM_PIN, 0);
    }

public:
    Motores()
    {
        left.PWM_PIN = 27;
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
        pinMode(left.PWM_PIN, OUTPUT);
        pinMode(left.IN1_PIN, OUTPUT);
        pinMode(left.IN2_PIN, OUTPUT);

        pinMode(right.PWM_PIN, OUTPUT);
        pinMode(right.IN1_PIN, OUTPUT);
        pinMode(right.IN2_PIN, OUTPUT);

        // Configurar encoders
        encoderLeft.attachHalfQuad(left.ENC_PIN_1, left.ENC_PIN_2);
        encoderRight.attachHalfQuad(right.ENC_PIN_1, right.ENC_PIN_2);

        encoderLeft.setCount(0);
        encoderRight.setCount(0);

        // Motor parado inicialmente
        stopMotors();
    }

    void lerComandos(char *commands)
    {
        for (int c = 0; c <= strlen(commands); c++)
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
            }
            delay(1000);
        }
    }

    void setSpeed(int newSpeed)
    {
        if (newSpeed >= 0 && newSpeed <= 100)
        {
            currentSpeed = newSpeed;
        }
    }
};