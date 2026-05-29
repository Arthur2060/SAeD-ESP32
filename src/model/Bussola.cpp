#include "headers/Bussola.h"

using namespace N;
using namespace std;

void Bussola::begin()
{
    configureCompass();
}

void Bussola::configureCompass()
{
    compass.begin();
}

float Bussola::collectCompassData()
{
    int16_t x, y, z;
    compass.getRawMagnetic(&x, &y, &z);

    float heading = atan2(y, x) * 180.0 / PI;
    return (int)heading % 360;
}