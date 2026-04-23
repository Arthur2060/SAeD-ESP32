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
        int mapRange(double x);
        const int ERROR_MARGE = 110;
    public:
        void begin();
        std::vector<uint> readColor();
        bool readColor(int vm, int vd, int vz);
        bool readColor(int vm, int vd, int vz, int vb);
        bool readColor(int valores[3]);
        bool readColor(int valores[4]);
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

int Color::mapRange(double x) {
    return (x - 0) * (255 - 0) / (1023 - 0) + 0;
}

bool Color::readColor(int vm, int vd, int vb) {
    int valores[3] = {readColor()[0], readColor()[1], readColor()[2]};
    
    if (
        (valores[0] - ERROR_MARGE) < vm && vm < (valores[0] + ERROR_MARGE) && 
        (valores[1] - ERROR_MARGE) < vd && vd < (valores[1] + ERROR_MARGE) && 
        (valores[2] - ERROR_MARGE) < vb && vb < (valores[2] + ERROR_MARGE)
    ) {
        return true;
    }

    return false;
}