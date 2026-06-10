#include "Bussola.h"

using namespace SAeD;
using namespace std;

bool Bussola::begin()
{
    compass.init();

    return true;
}

int Bussola::collectCompassData()
{
    compass.read();
    return compass.getAzimuth();
}