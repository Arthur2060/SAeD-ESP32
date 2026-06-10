#ifndef BUSSOLA_H
#define BUSSOLA_H

#include "QMC5883LCompass.h"
#include "Adafruit_Sensor.h"

namespace SAeD
{
    class Bussola
    {
    private:
        QMC5883LCompass compass;
        float rawTo360(float raw);

    public:
        bool begin();
        int collectCompassData();
    };
}
#endif
