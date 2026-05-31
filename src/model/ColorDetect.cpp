#include "ColorDetect.h"

using namespace SAeD;
using namespace std;

void ColorDetect::begin()
{
    pinMode(pinS0, OUTPUT);
    pinMode(pinS1, OUTPUT);
    pinMode(pinS2, OUTPUT);
    pinMode(pinS3, OUTPUT);
    pinMode(pinOut, INPUT);

    digitalWrite(pinS0, HIGH);
    digitalWrite(pinS1, LOW);

    calibrateColors();
}

std::vector<uint> ColorDetect::detectColors()
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

bool ColorDetect::isThisColor(color color)
{
    detectColors();

    if ((R >= color.minR && R <= color.MaxR) &&
        (G >= color.minG && G <= color.MaxG) &&
        (B >= color.minB && B <= color.MaxB))
    {
        return true;
    }

    return false;
}

void ColorDetect::calibrateColors()
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
        detectColors();
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
        detectColors();
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
        detectColors();
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

color ColorDetect::defineColor() {
    color newColor;

    newColor.MaxR = 0;
    newColor.MaxG = 0;
    newColor.MaxB = 0;
    newColor.minR = 999999;
    newColor.minG = 999999;
    newColor.minB = 999999;

    tempoInicioCalib = millis();
    do
    {
        detectColors();
        if (R > newColor.MaxR)
            newColor.MaxR = R;
        if (G > newColor.MaxG)
            newColor.MaxG = G;
        if (B > newColor.MaxB)
            newColor.MaxB = B;

        if (R < newColor.minR)
            newColor.minR = R;
        if (G < newColor.minG)
            newColor.minG = G;
        if (B < newColor.minB)
            newColor.minB = B;
    } while (!(millis() - tempoInicioCalib >= tempoCalibracao));

    return newColor;
}