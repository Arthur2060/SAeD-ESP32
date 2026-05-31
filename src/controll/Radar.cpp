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

std::vector<double> Radar::getRawSensors()
{
    double distance = 3.65 * ultrasom.collectUltrasonicData();
    double angle = bussola.collectCompassData();

    return {distance, angle};
}

std::vector<double> Radar::getObstacle()
{
    double distance = 3.65 * ultrasom.collectUltrasonicData();
    int angle = bussola.collectCompassData();

    if (distance >= 300)
    {
        return {0, 0};
    }

    return {distance * sin(angle), distance * cos(angle)};
}

std::vector<double> Radar::getObstacle(double distance, int angle)
{

    if (distance >= 300)
    {
        return {0, 0};
    }

    return {(distance * sin(angle)), (distance * cos(angle))};
}

std::vector<double> Radar::getObstacle(int angle)
{
    double distance = 3.65 * ultrasom.collectUltrasonicData();

    if (distance >= 300)
    {
        return {0, 0};
    }

    return {distance * sin(angle), distance * cos(angle)};
}