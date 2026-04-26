#include "model/Bussola.hpp"
#include "model/Ultrasom.hpp"
#include <vector>
#include <cmath>

class Radar {
    private:
        Ultrasom ultrasom;

    public:
        void begin() {
            ultrasom.begin(14, 13);
        }

        void begin(int trigger, int echo) {
            ultrasom.begin(trigger, echo);
        }

    std::vector<float> getObstacle(float angle) {
        float distance = ultrasom.collectUltrasonicData();

        return {distance * sin(angle), distance * cos(angle)};
    }
};