#ifndef BUSSOLA_H
#define BUSSOLA_H

#include "Adafruit_QMC5883P.h"
#include "Adafruit_Sensor.h"

namespace N
{
    class Bussola
    {
    private:
        Adafruit_QMC5883P compass;

        void configureCompass();

    public:
        void begin();
        float collectCompassData();
    };
}
#endif