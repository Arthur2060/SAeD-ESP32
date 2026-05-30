#ifndef COLOR_H
#define COLOR_H

#include "Arduino.h"
#include "vector"

// Pinos de conexao do modulo
#define pinS0 18
#define pinS1 19
#define pinS2 27
#define pinS3 13
#define pinOut 23

namespace N
{
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

        void begin();
        void calibrarCores();

        int detectarCor();
        std::vector<uint> getMacros();
        void setMacros(std::vector<uint> macros);

        std::vector<uint> detectaCores();
    };
}
#endif