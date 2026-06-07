#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <vector>

using namespace std;

namespace SAeD
{
    class MapManager
    {
    private:
        std::vector<int *> obstacles;

    public:
        int currentCell[2];

        MapManager();

        // Add an obstacle at cell coordinates (x,y). Returns true if added (was new).
        bool addObstacle(int x, int y);
    };
}

#endif
