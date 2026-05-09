#include "model/Bussola.hpp"
#include "model/Ultrasom.hpp"
#include <vector>
#include <cmath>

class Radar {
    private:
        Ultrasom ultrasom;
        Bussola bussola;

    public:
        void begin() {
            ultrasom.begin(14, 13);
            bussola.begin();
        }

        void begin(int trigger, int echo) {
            ultrasom.begin(trigger, echo);
        }

    std::vector<float> getObstacle() {
        float distance = ultrasom.collectUltrasonicData();
        float angle = bussola.collectCompassData();

        if (distance >= 300) {
            return {0, 0};
        }

        return {distance * sin(angle), distance * cos(angle)};
    }

    std::vector<float> getObstacle(float angle) {
        float distance = ultrasom.collectUltrasonicData();

        if (distance >= 300) {
            return {0, 0};
        }

        return {distance * sin(angle), distance * cos(angle)};
    }
};