#include "Bussola.h"

using namespace N;
using namespace std;

bool Bussola::begin()
{
    compass.init();

    // if (!compass.begin()) {
    //     return false;
    // }

    // compass.setDSR(QMC5883P_DSR_8);
    // compass.setMode(QMC5883P_MODE_NORMAL);
    // compass.setODR(QMC5883P_ODR_200HZ);
    // compass.setOSR(QMC5883P_OSR_8);
    // compass.setRange(QMC5883P_RANGE_8G);

    return true;
}

float Bussola::collectCompassData()
{
    compass.read();
    return compass.getAzimuth();

    // int16_t x, y, z;
    // compass.getRawMagnetic(&x, &y, &z);

    // float heading = atan2(y, x) * 180.0 / PI;
    // return (int)heading % 360;
}