#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <cmath>
#include <vector>

using namespace std;

namespace SAeD
{
    class MapManager
    {
    private:
        std::vector<int *> obstacles;
        float cellScale = 0.21;

    public:
        int currentCell[2];

        MapManager();

        // Add an obstacle at cell coordinates (x,y). Returns true if added (was new).
        bool addObstacle(int x, int y);
        char* createPath(int *end);
        void setCellScale(float cellScale);
    };
}

#endif
