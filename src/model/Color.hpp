#include "Arduino.h"
#include <vector>

//Pinos de conexao do modulo
#define pinS0 18
#define pinS1 19
#define pinS2 21
#define pinS3 22
#define pinOut 23
#define pinLED 26

//Pinos dos LEDs
#define pinoLedVerm 14
#define pinoLedVerd 12
#define pinoLedAzul 27

unsigned int valorVm = 0;
unsigned int valorVd = 0;
unsigned int valorAz = 0;
unsigned int valorBr = 0;

class Color {
    private:

    public:
        void begin();
        std::vector<uint> readColor();
};

void Color::begin()
{
  pinMode(pinOut, INPUT);

  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinLED, OUTPUT);
  
  pinMode(pinoLedVerm, OUTPUT);
  pinMode(pinoLedVerd, OUTPUT);
  pinMode(pinoLedAzul, OUTPUT);
  
  digitalWrite(pinS0, LOW);
  digitalWrite(pinS1, LOW);
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  digitalWrite(pinLED, LOW);

  digitalWrite(pinoLedVerm, LOW);
  digitalWrite(pinoLedVerd, LOW);
  digitalWrite(pinoLedAzul, LOW);
  
  digitalWrite(pinS1, LOW);
}

// *********** Função de leitura so sensor de cor ********************
std::vector<uint> Color::readColor() {
  valorVm = mapRange(pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH));
  
  valorVd = mapRange(pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH));
  
  valorAz = mapRange(pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH));
  
  valorBr = mapRange(pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH));

  return {valorVm, valorVd, valorAz, valorBr};
}

int mapRange(double x) {
    return (x - 0) * (255 - 0) / (1023 - 0) + 0;
}