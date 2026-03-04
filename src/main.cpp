#include <mpu6050.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <cmath>

const int tamanhoTabuleiro[2] = {10, 10};
int posicaoAtual[2] = {3, 4};

bool tabuleiro[10][10] = {};

float calcularDistanciaX();
float calcularDistanciaY();
void calcularCelula(float distanciaX, float distanciaY);

const float DIMENSAO_DE_QUADRO = 1.0;
const float DISTANCIA_MEDIDA = 12.0;

void setup()
{
  Serial.begin(9600);

  mpu_begin();

  Serial.println("Calibrando, deixa parado!");
  delay(1000);
  mpu_calibrate(200);
  Serial.println("===== Calibrado! =====\n");

  mpu_reset();
}

void loop()
{
  mpu_loop();

  Serial.printf("X = %.1f, Y = %.1f, Z = %.1f,\n", getAngleX(), getAngleY(), getAngleZ());

  // calcularCelula(accelerationX, DISTANCIA_MEDIDA);

  delay(1000);
}

float calcularDistanciaX(float angulo, float distancia)
{
  return (sin(angulo) * distancia);
}

float calcularDistanciaY(float angulo, float distancia)
{
  return (cos(angulo) * distancia);
}

void calcularCelula(float angulo, float distancia)
{
  float anguloEmGraus = angulo;

  int X = calcularDistanciaX(anguloEmGraus, distancia) - 1;
  int Y = calcularDistanciaY(anguloEmGraus, distancia) - 1;

  int targetX = abs(posicaoAtual[0] + Y);
  int targetY = abs(posicaoAtual[1] + X);

  Serial.printf("Celula: X=%i Y=%i\n", targetX, targetY);
  for (int linha = 0; linha <= tamanhoTabuleiro[0]; linha++)
  {
    for (int coluna = 0; coluna <= tamanhoTabuleiro[1]; coluna++)
    {
      if (linha == targetY && coluna == targetX)
      {
        Serial.print("[X]");
      }
      else if (linha == posicaoAtual[0] && coluna == posicaoAtual[1])
      {
        Serial.print("[O]");
      }
      else
      {
        Serial.print("[ ]");
      }
    }
    Serial.println("");
  }
}