#include "Radar.h"

using namespace SAeD;
using namespace std;

void Radar::begin()
{
    ultrasom.begin(12, 14);
    bussola.begin();
}

void Radar::begin(int trigger, int echo)
{
    ultrasom.begin(trigger, echo);
}

double *Radar::getRawSensors()
{
    double distance = 3.65 * ultrasom.collectUltrasonicData();
    double angle = bussola.collectCompassData();

    double result[2] = {distance, angle};

    return result;
    delete[] result;
}

double *Radar::getObstacle()
{
    double distance = ultrasom.collectUltrasonicData();
    int angle = bussola.collectCompassData();

    double result[2] = {distance * sin(angle), distance * cos(angle)};

    return result;
    delete[] result;
}

double *Radar::getObstacle(double distance, int angle)
{
    double result[2] = {(distance * sin(angle)), (distance * cos(angle))};

    return result;
    delete[] result;
}

double *Radar::getObstacle(int angle)
{
    double distance = 3.65 * ultrasom.collectUltrasonicData();

    double result[2] = {distance * sin(angle), distance * cos(angle)};

    return result;
    delete[] result;
}