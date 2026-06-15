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
}

uint* ColorDetect::detectColors()
{
    digitalWrite(pinS2, LOW);
    digitalWrite(pinS3, LOW);
    R = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

    digitalWrite(pinS2, HIGH);
    G = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

    digitalWrite(pinS2, LOW);
    digitalWrite(pinS3, HIGH);
    B = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

    uint result[3] = {R, G, B};

    return result;
    delete[] result;
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