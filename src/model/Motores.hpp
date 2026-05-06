#include <Arduino.h>
#include <ESP32Encoder.h>
#include <string.h>

class Motores
{
private:
    // Pinos dos Motores
    const int MOTOR_LEFT_IN1 = 2;
    const int MOTOR_LEFT_IN2 = 4;
    const int MOTOR_LEFT_PWM = 27;

    const int MOTOR_RIGHT_PWM = 13;
    const int MOTOR_RIGHT_IN1 = 18;
    const int MOTOR_RIGHT_IN2 = 19;

    const int MOTOR_LEFT_ENC_A = 26;
    const int MOTOR_LEFT_ENC_B = 25;

    const int MOTOR_RIGHT_ENC_A = 33;
    const int MOTOR_RIGHT_ENC_B = 32;

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
        Serial.println(">> MOVENDO PARA FRENTE");
        moveDistance(CELL_DISTANCE, 1); // 1 = para frente
    }

    void moveBackward()
    {
        Serial.println(">> MOVENDO PARA TRÁS");
        moveDistance(CELL_DISTANCE, -1); // -1 = para trás
    }

    void turnLeft()
    {
        Serial.println(">> VIRANDO 90° ANTI-HORÁRIO");
        rotate(90);
    }

    void turnRight()
    {
        Serial.println(">> VIRANDO 90° HORÁRIO");
        rotate(-90);
    }

    void spin360()
    {
        Serial.println(">> RODANDO 360°");
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
            setMotorDirection(MOTOR_LEFT_IN1, MOTOR_LEFT_IN2, 1); // Frente
            setMotorDirection(MOTOR_RIGHT_IN1, MOTOR_RIGHT_IN2, 1);
        }
        else
        {
            setMotorDirection(MOTOR_LEFT_IN1, MOTOR_LEFT_IN2, 0); // Trás
            setMotorDirection(MOTOR_RIGHT_IN1, MOTOR_RIGHT_IN2, 0);
        }

        int pwmValue = map(currentSpeed, 0, 100, MIN_PWM, MAX_PWM);
        analogWrite(MOTOR_LEFT_PWM, pwmValue);
        analogWrite(MOTOR_RIGHT_PWM, pwmValue);

        // Aguardar até atingir distância
        while (abs(encoderLeft.getCount()) < targetPulses && abs(encoderRight.getCount()) < targetPulses)
        {
            // Sincronização em malha fechada
            if (encoderLeft.getCount() > encoderRight.getCount() + 2)
            {
                analogWrite(MOTOR_RIGHT_PWM, pwmValue + 5);
                analogWrite(MOTOR_LEFT_PWM, pwmValue - 5);
            }
            else if (encoderRight.getCount() > encoderLeft.getCount() + 2)
            {
                analogWrite(MOTOR_LEFT_PWM, pwmValue + 5);
                analogWrite(MOTOR_RIGHT_PWM, pwmValue - 5);
            }
            delay(10);
        }

        stopMotors();
        Serial.print("Movimento concluído. Pulsos - Esq: ");
        Serial.print(encoderLeft.getCount());
        Serial.print(" | Dir: ");
        Serial.println(encoderRight.getCount());
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
            setMotorDirection(MOTOR_LEFT_IN1, MOTOR_LEFT_IN2, 1);
            setMotorDirection(MOTOR_RIGHT_IN1, MOTOR_RIGHT_IN2, 0);
        }
        else
        {
            // Anti-horário: direita frente, esquerda trás
            setMotorDirection(MOTOR_LEFT_IN1, MOTOR_LEFT_IN2, 0);
            setMotorDirection(MOTOR_RIGHT_IN1, MOTOR_RIGHT_IN2, 1);
        }

        int pwmValue = map(currentSpeed, 0, 100, MIN_PWM, MAX_PWM);
        analogWrite(MOTOR_LEFT_PWM, pwmValue);
        analogWrite(MOTOR_RIGHT_PWM, pwmValue);

        while (abs(encoderLeft.getCount()) < targetPulses || abs(encoderRight.getCount()) < targetPulses)
        {
            delay(10);
        }

        stopMotors();
        Serial.println("Rotação concluída!");
    }

    // ============================================================
    // CONTROLE DE VELOCIDADE
    // ============================================================

    void adjustSpeed()
    {
        Serial.println("Digite velocidade (0-100%):");
        while (!Serial.available())
            ;

        String speedStr = Serial.readStringUntil('\n');
        int newSpeed = speedStr.toInt();

        if (newSpeed >= 0 && newSpeed <= 100)
        {
            currentSpeed = newSpeed;
            Serial.print("Velocidade ajustada para: ");
            Serial.print(currentSpeed);
            Serial.println("%");
        }
        else
        {
            Serial.println("Velocidade inválida! Use 0-100");
        }
    }

    // ============================================================
    // FUNÇÕES AUXILIARES
    // ============================================================

    void setMotorDirection(int in1, int in2, int direction)
    {
        if (direction == 1)
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
        analogWrite(MOTOR_LEFT_PWM, 0);
        analogWrite(MOTOR_RIGHT_PWM, 0);
    }

    void printHelp()
    {
        Serial.println("\n=== COMANDOS DISPONÍVEIS ===");
        Serial.println("W - Mover para frente (1 célula = 0.3m)");
        Serial.println("S - Mover para trás");
        Serial.println("A - Virar 90° anti-horário");
        Serial.println("D - Virar 90° horário");
        Serial.println("R - Rodar 360°");
        Serial.println("V - Ajustar velocidade (0-100%)");
        Serial.println("H - Mostrar ajuda");
    }

public:
    Motores() {}

    void begin()
    {
        pinMode(MOTOR_LEFT_IN1, OUTPUT);
        pinMode(MOTOR_LEFT_IN2, OUTPUT);
        pinMode(MOTOR_LEFT_PWM, OUTPUT);

        pinMode(MOTOR_RIGHT_IN1, OUTPUT);
        pinMode(MOTOR_RIGHT_IN2, OUTPUT);
        pinMode(MOTOR_RIGHT_PWM, OUTPUT);

        // Configurar encoders
        encoderLeft.attachHalfQuad(MOTOR_LEFT_ENC_A, MOTOR_LEFT_ENC_B);
        encoderRight.attachHalfQuad(MOTOR_RIGHT_ENC_A, MOTOR_RIGHT_ENC_B);
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
            case 'V':
                adjustSpeed();
                break;
            case 'H':
                printHelp();
                break;
            default:
                Serial.println("Comando inválido!");
            }
            delay(1000);
        }
    }
};