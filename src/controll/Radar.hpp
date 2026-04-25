#include "model/Bussola.hpp"
#include "model/Ultrasom.hpp"
#include <vector>
#include <cmath>

class Radar {
    private:
        Bussola bussola;
        Ultrasom ultrasom;

        float lastDistance;
        float lastAngle;
    public:
        void begin(int trigger, int echo) {
            bussola.begin();
            ultrasom.begin(trigger, echo);
        }

    std::vector<float> getObstacle() {
        float distance = ultrasom.collectUltrasonicData();
        float angle = bussola.collectCompassData(); 
        

        return {distance * sin(angle), distance * cos(angle)};
    }
};