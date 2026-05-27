#include "Arduino.h"
#include "vector"

// Pinos de conexao do modulo
#define pinS0 18
#define pinS1 19
#define pinS2 21
#define pinS3 22
#define pinOut 23

class Color
{
private:
    unsigned int R = 0;
    unsigned int G = 0;
    unsigned int B = 0;

    //  VERMELHO
    unsigned int MRv = 0, MGv = 0, MBv = 0;
    unsigned int mRv = 999999, mGv = 999999, mBv = 999999;

    //  VERDE
    unsigned int MRg = 0, MGg = 0, MBg = 0;
    unsigned int mRg = 999999, mGg = 999999, mBg = 999999;

    //  AZUL
    unsigned int MRb = 0, MGb = 0, MBb = 0;
    unsigned int mRb = 999999, mGb = 999999, mBb = 999999;

    bool modoCalibracao = false;
    String corCalibrando = "";

    unsigned long tempoInicioCalib = 0;
    const int tempoCalibracao = 5000;

    std::vector<uint> CalibraVm();

public:
    Color() {}

    void begin()
    {
        pinMode(pinS0, OUTPUT);
        pinMode(pinS1, OUTPUT);
        pinMode(pinS2, OUTPUT);
        pinMode(pinS3, OUTPUT);
        pinMode(pinOut, INPUT);

        digitalWrite(pinS0, HIGH);
        digitalWrite(pinS1, LOW);
    }

    int detectarCores();

    std::vector<uint> detectaCorBruto();
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
    digitalWrite(pinS2, LOW);
    digitalWrite(pinS3, LOW);
    R = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

    digitalWrite(pinS2, HIGH);
    G = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

    digitalWrite(pinS2, LOW);
    digitalWrite(pinS3, HIGH);
    B = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
    return {R, G, B};
}

int Color::detectarCores()
{
    int cor = 0;

    //  Vermelho
    if ((R >= mRv && R <= MRv) &&
        (G >= mGv && G <= MGv) &&
        (B >= mBv && B <= MBv))
    {
        cor = 1;
    }

    //  Verde
    if ((R >= mRg && R <= MRg) &&
        (G >= mGg && G <= MGg) &&
        (B >= mBg && B <= MBg))
    {
        cor = 2;
    }

    //  Azul
    if ((R >= mRb && R <= MRb) &&
        (G >= mGb && G <= MGb) &&
        (B >= mBb && B <= MBb))
    {
        cor = 3;
    }

    return cor;
}