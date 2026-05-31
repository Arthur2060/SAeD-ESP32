#ifndef BUSSOLA_H
#define BUSSOLA_H

#include "QMC5883LCompass.h"
#include "Adafruit_Sensor.h"

namespace N
{
    class Bussola
    {
    private:
        QMC5883LCompass compass;

    public:
        bool begin();
        float collectCompassData();
    };
}
#endif