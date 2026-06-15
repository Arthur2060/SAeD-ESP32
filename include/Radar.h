#ifndef RADAR_H
#define RADAR_H

#include "Bussola.h"
#include "Ultrasom.h"
#include <cmath>

using namespace SAeD;

namespace SAeD
{
    class Radar
    {
    private:
        Ultrasom ultrasom;
        Bussola bussola;

    public:
        void begin();
        void begin(int trigger, int echo);

        double* getRawSensors();

        double* getObstacle();
        double* getObstacle(double distance, int angle);
        double* getObstacle(int angle);
    };
}

#endif
