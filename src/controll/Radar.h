#ifndef RADAR_H
#define RADAR_H

#include "model/Bussola.h"
#include "model/Ultrasom.h"
#include <vector>
#include <cmath>

using namespace N;

namespace N
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