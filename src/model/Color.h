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

    unsigned long tempoInicioCalib = 0;
    const int tempoCalibracao = 5000;

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

    void calibrarCores();

    int detectarCor();
    std::vector<uint> getMacros();
    void setMacros(std::vector<uint> macros);

    std::vector<uint> detectaCores();
};

std::vector<uint> Color::detectaCores()
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

int Color::detectarCor()
{
    detectaCores();

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

void Color::calibrarCores()
{
    // Vermelho

    Serial.println("Calibrando vermelho...");

    MRv = 0;
    MGv = 0;
    MBv = 0;
    mRv = 999999;
    mGv = 999999;
    mBv = 999999;

    tempoInicioCalib = millis();
    do
    {
        detectaCores();
        if (R > MRv)
            MRv = R;
        if (G > MGv)
            MGv = G;
        if (B > MBv)
            MBv = B;

        if (R < mRv)
            mRv = R;
        if (G < mGv)
            mGv = G;
        if (B < mBv)
            mBv = B;
    } while (!(millis() - tempoInicioCalib >= tempoCalibracao));

    Serial.println("Calibrado!");

    delay(500);

    // Verde

    Serial.println("Calibrando verde...");

    MRg = 0;
    MGg = 0;
    MBg = 0;
    mRg = 999999;
    mGg = 999999;
    mBg = 999999;

    tempoInicioCalib = millis();
    do
    {
        detectaCores();
        if (R > MRg)
            MRg = R;
        if (G > MGg)
            MGg = G;
        if (B > MBg)
            MBg = B;

        if (R < mRg)
            mRg = R;
        if (G < mGg)
            mGg = G;
        if (B < mBg)
            mBg = B;
    } while (!(millis() - tempoInicioCalib >= tempoCalibracao));

    Serial.println("Calibrado!");

    delay(500);

    // Azul

    Serial.println("Calibrando azul...");

    MRb = 0;
    MGb = 0;
    MBb = 0;
    mRb = 999999;
    mGb = 999999;
    mBb = 999999;

    tempoInicioCalib = millis();
    do
    {
        detectaCores();
        if (R > MRb)
            MRb = R;
        if (G > MGb)
            MGb = G;
        if (B > MBb)
            MBb = B;

        if (R < mRb)
            mRb = R;
        if (G < mGb)
            mGb = G;
        if (B < mBb)
            mBb = B;
    } while (!(millis() - tempoInicioCalib >= tempoCalibracao));

    Serial.println("Calibrado!");

    delay(500);
}

std::vector<uint> Color::getMacros()
{
    return
    {
        MRv, MGv, MBv, mRv, mGv, mBv,
        MRg, MGg, MBg, mRg, mGg, mBg,
        MRb, MGb, MBb, mRb, mGb, mBb
    };
}

void Color::setMacros(std::vector<uint> macros)
{
    MRv = macros[0];
    MGv = macros[1];
    MBv = macros[2];
    mRv = macros[3];
    mGv = macros[4];
    mBv = macros[5];

    MRg = macros[6];
    MGg = macros[7];
    MBg = macros[8];
    mRg = macros[9];
    mGg = macros[10];
    mBg = macros[11];

    MRb = macros[12];
    MGb = macros[13];
    MBb = macros[14];
    mRb = macros[15];
    mGb = macros[16];
    mBb = macros[17];
}