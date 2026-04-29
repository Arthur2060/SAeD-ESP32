#include <Arduino.h>
#include <ESP32Encoder.h> // Para leitura precisa dos encoders
#include <QuickPID.h>     // Para controle PID dos motores

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
// 2. CONSTANTES DO ROBÔ E DA ODOMETRIA
// ============================================================
// Parâmetros Físicos do Robô
const float RAIO_RODA = 0.0325;       // Raio da roda (metros)
const float DIST_ENTRE_RODAS = 0.150; // Largura entre rodas (metros)

// Parâmetros do Encoder (JGA25-370)
const float PULSOS_POR_REVOLUCAO_MOTOR = 11.0; // 11 pulsos por volta do eixo do motor
const float REDUCAO_GEARBOX = 34.0;            // Relação de redução do seu motor (exemplo)
// Pulsos por volta COMPLETA da roda:
const float PPR_RODA = PULSOS_POR_REVOLUCAO_MOTOR * REDUCAO_GEARBOX; // 374 pulsos/volta da roda
const float CIRCUNFERENCIA_RODA = 2.0 * PI * RAIO_RODA;              // ~0.204 metros
// Fator de conversão de pulsos do encoder para METROS
const float METROS_POR_PULSO = CIRCUNFERENCIA_RODA / PPR_RODA; // 0.204 / 374 = ~0.000545m

// Parâmetros do Movimento
const float DISTANCIA_POR_CELULA = 0.30;  // Distância para comando 'W' ou 'S' (0.3 metros)
const float ANGULO_GIRO_RAD = PI / 2.0;   // 90 graus em radianos (giro)
const float VELOCIDADE_PADRAO_LIN = 0.15; // Velocidade linear padrão = 0.15 m/s
const float VELOCIDADE_PADRAO_ANG = 1.0;  // Velocidade angular padrão = 1 rad/s (~57°/s)

// ============================================================
// 3. VARIÁVEIS GLOBAIS
// ============================================================
// Objetos dos Encoders (Biblioteca ESP32Encoder)
ESP32Encoder encoderEsquerdo;
ESP32Encoder encoderDireito;

// --- PID para o Motor Esquerdo ---
float setpointVelEsq = 0.0; // Velocidade desejada (m/s)
float inputVelEsq = 0.0;    // Velocidade real (m/s)
float outputPWM_Esq = 0.0;  // PWM calculado (-255 a 255)
// Ganhos PID (Ajuste os valores conforme o comportamento do seu robô!)
float Kp = 1.5, Ki = 8.0, Kd = 0.05;
QuickPID pidEsquerdo(&inputVelEsq, &outputPWM_Esq, &setpointVelEsq, Kp, Ki, Kd, QuickPID::Action::direct);

// --- PID para o Motor Direito ---
float setpointVelDir = 0.0;
float inputVelDir = 0.0;
float outputPWM_Dir = 0.0;
QuickPID pidDireito(&inputVelDir, &outputPWM_Dir, &setpointVelDir, Kp, Ki, Kd, QuickPID::Action::direct);

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
void atualizarOdometria();
void executarMovimentoReta(float distancia_m, float velocidade = VELOCIDADE_PADRAO_LIN);
void executarGiro(float angulo_rad, bool horario, float velocidade_ang = VELOCIDADE_PADRAO_ANG);
void processarComandoSerial();

// ============================================================
// 5. FUNÇÃO SETUP
// ============================================================
void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando Sistema de Robô Diferencial...");

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

  Serial.println("Sistema Inicializado. Aguardando comandos (W, A, S, D, R)...");
}

// ============================================================
// 6. FUNÇÃO LOOP PRINCIPAL
// ============================================================
void loop()
{
  // 1. Processa comandos vindos da Serial (Monitor)
  processarComandoSerial();

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
        Serial.println("Movimento de reta concluído!");
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
        Serial.println("Giro horário concluído!");
      }
      else if (!direcaoGiroHorario && anguloPercorridoAteInicio >= anguloAlvo)
      {
        setpointVelEsq = 0;
        setpointVelDir = 0;
        estadoAtual = PARADO;
        Serial.println("Giro anti-horário concluído!");
      }
    }
  }

  // Pequeno delay para evitar sobrecarga do processador
  delay(10);
}

// ============================================================
// 7. FUNÇÕES AUXILIARES
// ============================================================

/**
 * Aplica a potência de um motor usando sinais PWM e de direção.
 */
void aplicarPotenciaMotor(int pwm_pin, int in1, int in2, int potencia)
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
float calcularVelocidadeEsquerda()
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
float calcularVelocidadeDireita()
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
void atualizarPID()
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
void atualizarOdometria()
{
  unsigned long agora = millis();
  float deltaT = (agora - lastOdometryTime) / 1000.0;
  if (deltaT <= 0)
    return;

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
    float raio = (DIST_ENTRE_RODAS / 2.0) * (distDir + distEsq) / (distDir - distEsq);
    float deltaTheta = (distDir - distEsq) / DIST_ENTRE_RODAS;
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
}

/**
 * Executa um movimento em linha reta por uma distância específica.
 */
void executarMovimentoReta(float distancia_m, float velocidade)
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

/**
 * Executa um giro puro de um ângulo específico.
 */
void executarGiro(float angulo_rad, bool horario, float velocidade_ang)
{
  if (estadoAtual != PARADO)
    return;
  // Para girar, as rodas giram em velocidades iguais e opostas.
  // Velocidade linear das rodas = (velocidade_ang * DIST_ENTRE_RODAS) / 2.0
  float vel_linear_roda = (velocidade_ang * DIST_ENTRE_RODAS) / 2.0;
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

/**
 * Função para processar comandos recebidos pela Serial.
 */
void processarComandoSerial()
{
  if (Serial.available() > 0)
  {
    char comando = Serial.read();
    // Ignora caracteres que não são letras maiúsculas
    if (comando < 'A' || comando > 'Z')
      return;

    switch (comando)
    {
    case 'W':
      Serial.println("Comando W: Andar para frente");
      executarMovimentoReta(DISTANCIA_POR_CELULA);
      break;
    case 'S':
      Serial.println("Comando S: Andar para trás");
      executarMovimentoReta(-DISTANCIA_POR_CELULA);
      break;
    case 'A':
      Serial.println("Comando A: Girar 90° anti-horário");
      executarGiro(ANGULO_GIRO_RAD, false);
      break;
    case 'D':
      Serial.println("Comando D: Girar 90° horário");
      executarGiro(ANGULO_GIRO_RAD, true);
      break;
    case 'R':
      Serial.println("Comando R: Girar 360°");
      executarGiro(2 * PI, true);
      break;
    default:
      Serial.println("Comando inválido. Use W, A, S, D, R.");
      break;
    }
  }
}