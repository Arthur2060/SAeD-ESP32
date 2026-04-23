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

        int currentState = 0;

    public:
        void begin();
        std::vector<uint> readColor();
        bool readColor(int vm, int vd, int vz);
        bool readColor(int vm, int vd, int vz, int vb);
        bool readColor(int valores[3]);
        bool readColor(int valores[4]);
        void setState(int state);
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
  
  setState(2);

  digitalWrite(pinS2, HIGH);
  digitalWrite(pinS3, LOW);

  digitalWrite(pinLED, LOW);

  digitalWrite(pinoLedVerm, LOW);
  digitalWrite(pinoLedVerd, LOW);
  digitalWrite(pinoLedAzul, LOW);
  
  digitalWrite(pinS1, LOW);
}

// *********** Função de leitura so sensor de cor ********************
std::vector<uint> Color::readColor() {

  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);

  valorVm = mapRange(pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH));
  
  digitalWrite(pinS2, HIGH);
  digitalWrite(pinS3, HIGH);
  
  valorVd = mapRange(pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH));
  
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, HIGH);

  valorAz = mapRange(pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH));
  
  digitalWrite(pinS2, HIGH);
  digitalWrite(pinS3, LOW);

  valorBr = mapRange(pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH));

  return {valorVm, valorVd, valorAz, valorBr};
}

int Color::mapRange(double x) {
    return (x - 0) * (255 - 0) / (1023 - 0) + 0;
}

bool Color::readColor(int vm, int vd, int vz) {
    int valores[3] = {readColor()[0], readColor()[1], readColor()[2]};
    
    if (
        (valores[0] - ERROR_MARGE) < vm && vm < (valores[0] + ERROR_MARGE) && 
        (valores[1] - ERROR_MARGE) < vd && vd < (valores[1] + ERROR_MARGE) && 
        (valores[2] - ERROR_MARGE) < vz && vz < (valores[2] + ERROR_MARGE)
    ) {
        return true;
    }

    return false;
}

bool Color::readColor(int vm, int vd, int vz, int vb) {
    int valorBranco = readColor()[3];
    bool resul = readColor(vm, vd, vz);

    if (
        resul &&
        (valorBranco - ERROR_MARGE) < vb && vb < (valorBranco + ERROR_MARGE)
    ) {
        return true;
    }

    return false;
}

bool Color::readColor(int valores[3]) {
    return readColor(valores[0], valores[1], valores[2]);
}

bool Color::readColor(int valores[4]) {
    return readColor(valores[0], valores[1], valores[2], valores[3]);
}

void Color::setState(int state) {
    switch (state)
    {
    case 0:
        digitalWrite(pinS0, LOW);
        digitalWrite(pinS1, LOW);
        break;
        
    case 1:
        digitalWrite(pinS0, LOW);
        digitalWrite(pinS1, HIGH);
        break;
    case 2:
        digitalWrite(pinS0, HIGH);
        digitalWrite(pinS1, LOW);
        break;
        
    case 3:
        digitalWrite(pinS0, HIGH);
        digitalWrite(pinS1, HIGH);
        break;
        
    default:
        return;
        break;
    }
    currentState = state;
}