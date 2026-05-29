#include "model/Bussola.h"
#include "model/Ultrasom.h"
#include <vector>
#include <cmath>

class Radar
{
private:
    Ultrasom ultrasom;
    Bussola bussola;

public:
    void begin()
    {
        ultrasom.begin(12, 14);
        bussola.begin();
    }

    void begin(int trigger, int echo)
    {
        ultrasom.begin(trigger, echo);
    }

    std::vector<double> getRawSensors()
    {
        double distance = 3.65 * ultrasom.collectUltrasonicData();
        double angle = bussola.collectCompassData();
        
        return {distance, angle};
    }
    
    std::vector<double> getObstacle()
    {
        double distance = 3.65 * ultrasom.collectUltrasonicData();
        int angle = bussola.collectCompassData();
        
        if (distance >= 300)
        {
            return {0, 0};
        }
        
        return {distance * sin(angle), distance * cos(angle)};
    }
    
    std::vector<double> getObstacle(double distance, int angle)
    {
        
        if (distance >= 300)
        {
            return {0, 0};
        }
        
        return {(distance * sin(angle)), (distance * cos(angle))};
    }
    
    std::vector<double> getObstacle(int angle)
    {
        double distance = 3.65 * ultrasom.collectUltrasonicData();
        
        if (distance >= 300)
        {
            return {0, 0};
        }

        return {distance * sin(angle), distance * cos(angle)};
    }
};