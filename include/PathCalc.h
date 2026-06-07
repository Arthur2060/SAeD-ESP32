#ifndef PATH_CALC_H
#define PATH_CALC_H

#include <vector>
#include <cmath>

namespace SAeD
{
    class PathCalc
    {
    private:
    public:
        std::vector<char> createPath(int *currentCell, int *targetCell);
    };
}

#endif
