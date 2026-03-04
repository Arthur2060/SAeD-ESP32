#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <Wire.h>
#include <cmath>

Adafruit_MPU6050 mpu;
sensors_event_t g, a, t;

char* ssid = "Arthur";
char* password = "arthur123";

const int tamanhoTabuleiro[2] = {10, 10};
int posicaoAtual[2] = {3, 4};

bool tabuleiro[10][10] = {};

float calcularDistanciaX();
float calcularDistanciaY();
void calcularCelula(float distanciaX, float distanciaY);

const float DIMENSAO_DE_QUADRO = 1.0;
const float DISTANCIA_MEDIDA = 12.0;

void setup() {
  Serial.begin(9600);
  if (!mpu.begin()) {
    Serial.println("Erro ao carregar MPU6050");
  }
}

void loop() {
  mpu.getEvent(&g, &a, &t);

  float accelerationX = atan2(a.acceleration.x,
                     sqrt(a.acceleration.y * a.acceleration.y +
                          a.acceleration.z * a.acceleration.z)) * 180.0 / PI;

  Serial.printf("X = %f\n", accelerationX);
  
  calcularCelula(30, DISTANCIA_MEDIDA);
  
  delay(1000);
}

float calcularDistanciaX(float angulo, float distancia) {
  return (sin(angulo) * distancia);
}

float calcularDistanciaY(float angulo, float distancia) {
  return (cos(angulo) * distancia);
}

void calcularCelula(float angulo, float distancia)
{
  float anguloEmGraus = angulo * (M_PI / 180.0);
    
  int X = calcularDistanciaX(anguloEmGraus, distancia);
  int Y = calcularDistanciaY(anguloEmGraus, distancia);

  int targetX = abs(posicaoAtual[0] + Y);
  int targetY = abs(posicaoAtual[1] + X);
  
  Serial.printf("Celula: X=%i Y=%i\n", X, Y);
  for(int linha = 0 ; linha <= tamanhoTabuleiro[0] ; linha++) {
    for(int coluna = 0 ; coluna <= tamanhoTabuleiro[1] ; coluna++) {
      if (linha == targetY, coluna == targetX) {
        Serial.print("[ ]");
      } else {
        if (linha == posicaoAtual[0] && coluna == posicaoAtual[1]) {
          Serial.print("[O]");
        } else {
          Serial.print("[X]");
        }
      }
    }
    Serial.println("");
  }
}