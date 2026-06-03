#ifndef COLOR_H
#define COLOR_H

#include "Arduino.h"
#include "vector"

// Pinos de conexao do modulo
#define pinS0 18
#define pinS1 19
#define pinS2 27
#define pinS3 5
#define pinOut 23

namespace SAeD
{
    struct color
    {
        unsigned int MaxR, MaxG, MaxB, minR, minG, minB;
    };

    class ColorDetect
    {
    private:
        unsigned int R = 0;
        unsigned int G = 0;
        unsigned int B = 0;

        unsigned long tempoInicioCalib = 0;
        const int tempoCalibracao = 5000;

    public:
        void begin();

        std::vector<uint> detectColors();
        bool isThisColor(color color);
        color defineColor();
    };
}
#endif