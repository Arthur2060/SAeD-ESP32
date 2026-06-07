#ifndef RADAR_H
#define RADAR_H

#include "Bussola.h"
#include "Ultrasom.h"
#include <vector>
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

        std::vector<double> getRawSensors();

        std::vector<double> getObstacle();
        std::vector<double> getObstacle(double distance, int angle);
        std::vector<double> getObstacle(int angle);
    };
}

#endif
