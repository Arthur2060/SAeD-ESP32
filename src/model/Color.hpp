#include "Arduino.h"
#include "vector"

// Pinos de conexao do modulo
#define pinS0 18
#define pinS1 19
#define pinS2 21
#define pinS3 22
#define pinOut 23
#define btnCalibra 26

// Pinos dos LEDs
#define pinoLedVerm 14
#define pinoLedVerd 12
#define pinoLedAzul 27

class Color
{
private:
    unsigned int R = 0;
    unsigned int G = 0;
    unsigned int B = 0;

    unsigned int Cor;

    unsigned int MRvermelho = 0;
    unsigned int MGvermelho = 0;
    unsigned int MBvermelho = 0;
    unsigned int mRvermelho = 0;
    unsigned int mGvermelho = 0;
    unsigned int mBvermelho = 0;

    std::vector<uint> CalibraVm();
    std::vector<uint> detectaCorBruto();

public:
    Color() {}

    void begin()
    {
        pinMode(pinS0, OUTPUT);
        pinMode(pinS1, OUTPUT);
        pinMode(pinS2, OUTPUT);
        pinMode(pinS3, OUTPUT);
        pinMode(btnCalibra, INPUT);
        pinMode(pinOut, INPUT);

        pinMode(pinoLedVerm, OUTPUT);
        pinMode(pinoLedVerd, OUTPUT);
        pinMode(pinoLedAzul, OUTPUT);

        digitalWrite(pinS0, HIGH);
        digitalWrite(pinS1, LOW);

        delay(2000);
    }

    std::vector<uint> detectaCor();
};

// *********** Função de leitura so sensor de cor ********************
std::vector<uint> Color::detectaCorBruto()
{
    // Vermelho
    digitalWrite(pinS2, LOW);
    digitalWrite(pinS3, LOW);
    R = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

    // Verde
    digitalWrite(pinS2, HIGH);
    G = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

    // Azul
    digitalWrite(pinS2, LOW);
    digitalWrite(pinS3, HIGH);
    B = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
    return {R, G, B};
}

std::vector<uint> Color::CalibraVm()
{
    std::vector<uint> cores = detectaCorBruto();
    unsigned int Cor, Mvm, Mvd, Maz, mvm, mvd, maz, compR, compG, compB;
    compR = cores[0];
    compG = cores[1];
    compB = cores[2];

    if (R < G && R < B)
    {
        // Salva o maior e o menor valor R lido durante calibração
        if (R < compR)
        {
            Mvm = compR;
        }

        if (R > compR)
        {
            mvm = compR;
        }

        // Salva o maior e o menor valor G lido durante calibração
        if (G < compG)
        {
            Mvd = compG;
        }

        if (G > compG)
        {
            mvd = compG;
        }

        // Salva o maior e o menor valor B lido durante calibração
        if (B < compB)
        {
            Maz = compB;
        }

        if (B > compB)
        {
            maz = compB;
        }
    }

    return {Mvm, Mvd, Maz, mvm, mvd, maz};
}

std::vector<uint> Color::detectaCor()
{
    while (btnCalibra == 1)
    {
        CalibraVm();
    }
    // Detecta a cor
    detectaCorBruto();

    std::vector<uint> redefine = CalibraVm();
    MRvermelho = redefine[0];
    MGvermelho = redefine[1];
    MBvermelho = redefine[2];
    mRvermelho = redefine[3];
    mGvermelho = redefine[4];
    mBvermelho = redefine[5];

    // Verifica se a cor vermelha foi detectada
    if (
        R < MRvermelho && R > mRvermelho &&
        G < MGvermelho && G > mGvermelho &&
        B < MBvermelho && B > mBvermelho)
    {
        digitalWrite(pinoLedVerm, HIGH); // Acende o led vermelho
        digitalWrite(pinoLedVerd, LOW);
        digitalWrite(pinoLedAzul, LOW);
    }

    // Delay para apagar os leds e reiniciar o processo
    delay(50);
    digitalWrite(pinoLedVerm, LOW);
    digitalWrite(pinoLedVerd, LOW);
    digitalWrite(pinoLedAzul, LOW);

    // Retorna valores para a classe pai
    return {R, G, B};
}