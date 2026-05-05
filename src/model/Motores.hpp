// Aqui eu farei a programação do motor, onde ele irá receber a velocidade 
// e a direção, e irá controlar o motor de acordo com isso.
#include <Arduino.h>
#include <ESP32Encoder.h> // Para leitura precisa dos encoders
#include <QuickPID.h>     // Para controle PID dos motores
#include <vector>

// ============================================================
// 1. DEFINIÇÕES DOS PINOS (HARDWARE)
// ============================================================

// Motor Esquerdo (Ponte H)
#define PIN_MOTOR_L_PWM 27
#define PIN_MOTOR_L_IN1 2
#define PIN_MOTOR_L_IN2 4
// Motor Direito
#define PIN_MOTOR_R_PWM 19
#define PIN_MOTOR_R_IN3 18
#define PIN_MOTOR_R_IN4 32

// Encoders (Sinais dos canais)
#define PIN_ENC_L_A 26
#define PIN_ENC_L_B 25
#define PIN_ENC_R_A 33
#define PIN_ENC_R_B 31

// ============================================================
// 2. VARIÁVEIS GLOBAIS
// ============================================================
// Objetos dos Encoders e PID (Biblioteca ESP32Encoder e QuickPID)
ESP32Encoder encoderEsquerdo;
ESP32Encoder encoderDireito;
QuickPID pidEsquerdo;

class Motores
{
private:
    // ============================================================
    // 2. CONSTANTES DO ROBÔ E DA ODOMETRIA
    // ============================================================
    // Parâmetros Físicos do Robô
    float raioDaRoda;       // Raio da roda (metros)
    float distanciaEntreRodas; // Largura entre rodas (metros)

    // Parâmetros do Encoder (JGA25-370)
    const float PULSOS_POR_REVOLUCAO_MOTOR = 11.0; // 11 pulsos por volta do eixo do motor
    const float REDUCAO_GEARBOX = 34.0;            // Relação de redução do seu motor (exemplo)
    // Pulsos por volta COMPLETA da roda:
    const float PPR_RODA = PULSOS_POR_REVOLUCAO_MOTOR * REDUCAO_GEARBOX; // 374 pulsos/volta da roda
    float CIRCUNFERENCIA_RODA;              // ~0.204 metros
    // Fator de conversão de pulsos do encoder para METROS
    const float METROS_POR_PULSO = CIRCUNFERENCIA_RODA / PPR_RODA; // 0.204 / 374 = ~0.000545m

    // Parâmetros do Movimento
    float distanciaPorCelula;  // Distância para comando 'W' ou 'S' (0.3 metros)
    const float ANGULO_GIRO_RAD = PI / 2.0;   // 90 graus em radianos (giro)
    float velocidadePadraoLinear = 0.15; // Velocidade linear padrão = 0.15 m/s
    float velocidadePadraoAngular = 1.0;  // Velocidade angular padrão = 1 rad/s (~57°/s)

    // --- PID para o Motor Esquerdo ---
    float setpointVelEsq = 0.0; // Velocidade desejada (m/s)
    float inputVelEsq = 0.0;    // Velocidade real (m/s)
    float outputPWM_Esq = 0.0;  // PWM calculado (-255 a 255)
    // Ganhos PID (Ajuste os valores conforme o comportamento do seu robô!)
    float Kp = 1.5;
    float Ki = 8.0;
    float Kd = 0.05;

    // --- PID para o Motor Direito ---
    float setpointVelDir = 0.0;
    float inputVelDir = 0.0;
    float outputPWM_Dir = 0.0;
    QuickPID pidDireito;

    // --- Variáveis para Cálculo de Velocidade Real ---
    unsigned long lastTimeEsq = 0, lastTimeDir = 0;
    float lastCountEsq = 0, lastCountDir = 0;

    // --- Variáveis de Navegação (Odometria) ---
    float posX = 0, posY = 0, theta = 0; // Posição (X, Y) e Orientação (rad)
    unsigned long lastOdometryTime = 0;

    // --- Variáveis de Estado dos Movimentos ---
    enum EstadoMovimento
    {
        PARADO,
        ANDANDO_RETA,
        GIRANDO
    };
    EstadoMovimento estadoAtual = PARADO;
    float distanciaAlvo = 0;
    float anguloAlvo = 0;
    float distanciaPercorridaInicio = 0;
    float anguloPercorridoInicio = 0;

    // --- Variável de Controle do Giro ---
    bool direcaoGiroHorario = true; // true = Horário (Direita), false = Anti-horário

    // --- Filtro simples para suavizar a leitura de velocidade (opcional) ---
    float velocidadeEsqFiltrada = 0;
    float velocidadeDirFiltrada = 0;
    const float ALPHA = 0.3; // Fator de suavização: quanto menor, mais suave

    // ============================================================
    // 4. PROTÓTIPOS DAS FUNÇÕES
    // ============================================================
    void aplicarPotenciaMotor(int pwm_pin, int in1, int in2, int potencia);
    float calcularVelocidadeEsquerda();
    float calcularVelocidadeDireita();
    void atualizarPID();
    void executarMovimentoReta(float distancia_m, float velocidade);
    void executarMovimentoReta(float distancia_m);
    void executarGiro(float angulo_rad, bool horario, float velocidade_ang);
    void movimentarCarro();

public:
    Motores()
    {
        QuickPID pidEsquerdo(&inputVelEsq, &outputPWM_Esq, &setpointVelEsq, Kp, Ki, Kd, QuickPID::Action());
        QuickPID pidDireito(&inputVelDir, &outputPWM_Dir, &setpointVelDir, Kp, Ki, Kd, QuickPID::Action());
    }

    void begin(float distanciaEntreRodas, float raioDaRoda, float distanciaPorCelula)
    {
        distanciaEntreRodas = distanciaEntreRodas;
        raioDaRoda = raioDaRoda;
        distanciaPorCelula = distanciaPorCelula;

        
        CIRCUNFERENCIA_RODA = 2.0 * PI * raioDaRoda; // ~0.204 metros
        // --- Inicialização dos Pinos dos Motores ---
        pinMode(PIN_MOTOR_L_PWM, OUTPUT);
        pinMode(PIN_MOTOR_L_IN1, OUTPUT);
        pinMode(PIN_MOTOR_L_IN2, OUTPUT);
        pinMode(PIN_MOTOR_R_PWM, OUTPUT);
        pinMode(PIN_MOTOR_R_IN3, OUTPUT);
        pinMode(PIN_MOTOR_R_IN4, OUTPUT);

        // --- Inicialização dos Encoders (Biblioteca ESP32Encoder) ---
        encoderEsquerdo.attachHalfQuad(PIN_ENC_L_A, PIN_ENC_L_B);
        encoderDireito.attachFullQuad(PIN_ENC_R_A, PIN_ENC_R_B);
        encoderEsquerdo.clearCount();
        encoderDireito.clearCount();

        // --- Inicialização dos Controladores PID ---
        // Define os limites de saída da potência (-255 a 255)
        pidEsquerdo.SetOutputLimits(-255, 255);
        pidDireito.SetOutputLimits(-255, 255);
        // Configura o PID para o modo automático
        pidEsquerdo.SetMode(QuickPID::Control::automatic);
        pidDireito.SetMode(QuickPID::Control::automatic);
        // Desabilita o anti-windup por enquanto (padrão é adequado)
        pidEsquerdo.SetAntiWindupMode(QuickPID::iAwMode::iAwClamp);
        pidDireito.SetAntiWindupMode(QuickPID::iAwMode::iAwClamp);

        // --- Tempos Iniciais para Velocidade e Odometria ---
        lastTimeEsq = micros();
        lastTimeDir = micros();
        lastOdometryTime = millis();
    }

    void begin()
    {
        begin(0.0026, 0.00256,  0.30);
    }

    void begin(float distanciaPorCelula)
    {
        begin(0.0026, 0.00256, distanciaPorCelula);
    }

    void processarComando(std::vector<char> commands);
    void executarGiro(float angulo_rad, bool horario);
    void executarGiro(float angulo_rad);

    float atualizarOdometria();

    float getVelocidadeLinear() {
        return this->velocidadePadraoLinear;
    }
    
    float getVelocidadeAngular() {
        return this->velocidadePadraoAngular;
    }
    
    void setVelocidadeLinear(float newVel) {
        this->velocidadePadraoLinear = newVel;
    }

    void setVelocidadeAngular(float newVel) {
        this->velocidadePadraoAngular = newVel;
    }

    void setDistanciaPorCelula(float newDistanciaPorCelula) {
        this->distanciaPorCelula = newDistanciaPorCelula;
    }
};

// ============================================================
// 4. FUNÇÕES
// ============================================================

/**
 * Aplica a potência de um motor usando sinais PWM e de direção.
 */
void Motores::aplicarPotenciaMotor(int pwm_pin, int in1, int in2, int potencia)
{
    if (potencia >= 0)
    {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        analogWrite(pwm_pin, potencia);
    }
    else
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        analogWrite(pwm_pin, -potencia);
    }
}

/**
 * Calcula a velocidade real do motor esquerdo em m/s usando o encoder.
 */
float Motores::calcularVelocidadeEsquerda()
{
    unsigned long now = micros();
    long countNow = encoderEsquerdo.getCount();
    float deltaT = (now - lastTimeEsq) / 1000000.0;
    if (deltaT <= 0)
        return 0;

    long deltaCount = countNow - lastCountEsq;
    float distanciaPercorrida = deltaCount * METROS_POR_PULSO;
    float velocidade = distanciaPercorrida / deltaT;

    lastCountEsq = countNow;
    lastTimeEsq = now;

    velocidadeEsqFiltrada = (ALPHA * velocidade) + ((1 - ALPHA) * velocidadeEsqFiltrada);
    return velocidadeEsqFiltrada;
}

/**
 * Calcula a velocidade real do motor direito em m/s usando o encoder.
 */
float Motores::calcularVelocidadeDireita()
{
    unsigned long now = micros();
    long countNow = encoderDireito.getCount();
    float deltaT = (now - lastTimeDir) / 1000000.0;
    if (deltaT <= 0)
        return 0;

    long deltaCount = countNow - lastCountDir;
    float distanciaPercorrida = deltaCount * METROS_POR_PULSO;
    float velocidade = distanciaPercorrida / deltaT;

    lastCountDir = countNow;
    lastTimeDir = now;

    velocidadeDirFiltrada = (ALPHA * velocidade) + ((1 - ALPHA) * velocidadeDirFiltrada);
    return velocidadeDirFiltrada;
}

/**
 * Atualiza os PIDs e aplica a potência calculada aos motores.
 */
void Motores::atualizarPID()
{
    // 1. Lê as velocidades reais
    inputVelEsq = calcularVelocidadeEsquerda();
    inputVelDir = calcularVelocidadeDireita();

    // 2. Executa o cálculo PID
    pidEsquerdo.Compute();
    pidDireito.Compute();

    // 3. Aplica a potência PWM aos motores
    aplicarPotenciaMotor(PIN_MOTOR_L_PWM, PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2, (int)outputPWM_Esq);
    aplicarPotenciaMotor(PIN_MOTOR_R_PWM, PIN_MOTOR_R_IN3, PIN_MOTOR_R_IN4, (int)outputPWM_Dir);
}

/**
 * Atualiza a pose do robô (x, y, theta) usando odometria diferencial.
 */
float Motores::atualizarOdometria()
{
    unsigned long agora = millis();
    float deltaT = (agora - lastOdometryTime) / 1000.0;
    if (deltaT <= 0)
        return 0;

    // Distâncias percorridas por cada roda no último intervalo
    float distEsq = (setpointVelEsq * deltaT);
    float distDir = (setpointVelDir * deltaT);

    // Atualiza a odometria usando o modelo diferencial
    if (fabs(distDir - distEsq) < 1e-6)
    {
        // Movimento em linha reta
        posX += distDir * cos(theta);
        posY += distDir * sin(theta);
    }
    else
    {
        float raio = (distanciaEntreRodas / 2.0) * (distDir + distEsq) / (distDir - distEsq);
        float deltaTheta = (distDir - distEsq) / distanciaEntreRodas;
        float deltaX = raio * (sin(theta + deltaTheta) - sin(theta));
        float deltaY = raio * (cos(theta) - cos(theta + deltaTheta));
        posX += deltaX;
        posY += deltaY;
        theta += deltaTheta;

        // Normaliza o ângulo entre 0 e 2*PI
        if (theta < 0)
            theta += 2 * PI;
        if (theta >= 2 * PI)
            theta -= 2 * PI;
    }

    lastOdometryTime = agora;
    return theta;
}

/**
 * Executa um movimento em linha reta por uma distância específica.
 */
void Motores::executarMovimentoReta(float distancia_m, float velocidade)
{
    if (estadoAtual != PARADO)
        return;
    setpointVelEsq = velocidade;
    setpointVelDir = velocidade;
    distanciaAlvo = distancia_m;
    posX = 0;
    posY = 0;
    theta = 0; // Reinicia odometria para este movimento
    estadoAtual = ANDANDO_RETA;
}

void Motores::executarMovimentoReta(float distancia_m)
{
    if (estadoAtual != PARADO)
        return;
    setpointVelEsq = velocidadePadraoLinear;
    setpointVelDir = velocidadePadraoLinear;
    distanciaAlvo = distancia_m;
    posX = 0;
    posY = 0;
    theta = 0; // Reinicia odometria para este movimento
    estadoAtual = ANDANDO_RETA;
}

/**
 * Executa um giro puro de um ângulo específico.
 */
void Motores::executarGiro(float angulo_rad, bool horario, float velocidade_ang)
{
    if (estadoAtual != PARADO)
        return;
    // Para girar, as rodas giram em velocidades iguais e opostas.
    // Velocidade linear das rodas = (velocidade_ang * distanciaEntreRodas) / 2.0
    float vel_linear_roda = (velocidade_ang * distanciaEntreRodas) / 2.0;
    if (horario)
    {
        setpointVelEsq = vel_linear_roda;
        setpointVelDir = -vel_linear_roda;
    }
    else
    {
        setpointVelEsq = -vel_linear_roda;
        setpointVelDir = vel_linear_roda;
    }
    anguloAlvo = angulo_rad;
    direcaoGiroHorario = horario;
    anguloPercorridoInicio = theta;
    estadoAtual = GIRANDO;
}
void Motores::executarGiro(float angulo_rad, bool horario)
{
    if (estadoAtual != PARADO)
        return;
    // Para girar, as rodas giram em velocidades iguais e opostas.
    // Velocidade linear das rodas = (velocidade_ang * distanciaEntreRodas) / 2.0
    float vel_linear_roda = (velocidadePadraoAngular * distanciaEntreRodas) / 2.0;
    if (horario)
    {
        setpointVelEsq = vel_linear_roda;
        setpointVelDir = -vel_linear_roda;
    }
    else
    {
        setpointVelEsq = -vel_linear_roda;
        setpointVelDir = vel_linear_roda;
    }
    anguloAlvo = angulo_rad;
    direcaoGiroHorario = horario;
    anguloPercorridoInicio = theta;
    estadoAtual = GIRANDO;
}

void Motores::executarGiro(float angulo_rad)
{
    executarGiro(angulo_rad, true);
}

/**
 * Função para processar comandos recebidos
 */
void Motores::processarComando(std::vector<char> commands)
{
    for (char command : commands)
    {
        // Ignora caracteres que não são letras maiúsculas
        if (command < 'A' || command > 'Z')
            return;

        switch (command)
        {
        case 'W':
            executarMovimentoReta(distanciaPorCelula);
            break;
        case 'S':
            executarMovimentoReta(-distanciaPorCelula);
            break;
        case 'A':
            executarGiro(ANGULO_GIRO_RAD, false);
            break;
        case 'D':
            executarGiro(ANGULO_GIRO_RAD, true);
            break;
        case 'R':
            executarGiro(2 * PI, true);
            break;
        default:
            return;
            break;
        }
        movimentarCarro();
    }
}

void Motores::movimentarCarro()
{
    // 2. Atualiza os PIDs e envia potência aos motores
    atualizarPID();

    // 3. Atualiza a posição do robô por odometria
    atualizarOdometria();

    // 4. Lógica de controle de movimento (para parar ao atingir o alvo)
    if (estadoAtual != PARADO)
    {
        if (estadoAtual == ANDANDO_RETA)
        {
            float distanciaPercorrida = sqrt(pow(posX, 2) + pow(posY, 2)); // Simplificação
            if (distanciaPercorrida >= distanciaAlvo)
            {
                setpointVelEsq = 0;
                setpointVelDir = 0;
                estadoAtual = PARADO;
            }
        }
        else if (estadoAtual == GIRANDO)
        {
            float anguloPercorridoAteInicio = theta - anguloPercorridoInicio;
            if (direcaoGiroHorario && anguloPercorridoAteInicio <= -anguloAlvo)
            {
                setpointVelEsq = 0;
                setpointVelDir = 0;
                estadoAtual = PARADO;
            }
            else if (!direcaoGiroHorario && anguloPercorridoAteInicio >= anguloAlvo)
            {
                setpointVelEsq = 0;
                setpointVelDir = 0;
                estadoAtual = PARADO;
            }
        }
    }

    // Pequeno delay para evitar sobrecarga do processador
    delay(10);
}