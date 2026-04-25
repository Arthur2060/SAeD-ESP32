#include <Arduino.h>
#include <RoboCore_Vespa.h>
#include <cmath>

class Garra {
    private:
    VespaServo servos[4];
    void moverCosseno(int servoIdx, int de, int para, int passos);
    void home();
    
    const uint16_t SERVO_MAX = 2500;
    const uint16_t SERVO_MIN = 500;
    
    const uint16_t SERVO_BASE = 0;
    const uint16_t SERVO_ALTURA = 1;
    const uint16_t SERVO_EXTENSOR = 2;
    const uint16_t SERVO_GARRA = 3;

    public:
        void begin() {
            servos[SERVO_GARRA].attach(VESPA_SERVO_S1, SERVO_MIN, SERVO_MAX);
            servos[SERVO_EXTENSOR].attach(VESPA_SERVO_S2, SERVO_MIN, SERVO_MAX);
            servos[SERVO_ALTURA].attach(VESPA_SERVO_S3, SERVO_MIN, SERVO_MAX);
            servos[SERVO_BASE].attach(VESPA_SERVO_S4, SERVO_MIN, SERVO_MAX);

            home();
        }
};

void Garra::moverCosseno(int servoIdx, int de, int para, int passos) {
  for (int i = 0; i <= passos; i++) {
    // Cálculo da curva de 0 a 1 usando Cosseno (Ease In-Out)
    float t = (float)i / (float)passos;
    float curva = (1.0 - cos(t * PI)) / 2.0;
    
    // Interpolação entre os ângulos usando a curva
    float anguloAtual = de + (para - de) * curva;
    
    servos[servoIdx].write(anguloAtual);
    
    // 20ms é o tempo de atualização real de um servo (50Hz)
    // Menos que isso o servo ignora, mais que isso ele treme.
    delay(20); 
  }
}

void Garra::home() {
  servos[SERVO_EXTENSOR].write(100);
  servos[SERVO_BASE].write(90);
  servos[SERVO_ALTURA].write(60);
  servos[SERVO_GARRA].write(30);
}