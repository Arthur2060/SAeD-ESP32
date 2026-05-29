#ifndef PATH_CALC_H
#define PATH_CALC_H

#include <vector>
#include <cmath>

namespace N
{
    class PathCalc
    {
    private:
        std::vector<std::vector<bool>> map;
        int targetCell[2] = {0, 0};

    public:
        int currentCell[2] = {0, 0};

        PathCalc() {}
        PathCalc(std::vector<std::vector<bool>> map);
        PathCalc(int scaleX, int scaleY);

        std::vector<int> getTarget();
      
        std::vector<char> setTarget(int *target);
        std::vector<char> setTarget(int x, int y);
        
        std::vector<char> moveToTarget();

        void setMap(std::vector<std::vector<bool>> map);
    };
}

#endif