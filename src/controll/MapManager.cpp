#include "MapManager.h"

using namespace SAeD;
using namespace std;

MapManager::MapManager()
{
    this->currentCell[0] = 0;
    this->currentCell[1] = 0;
}
bool MapManager::addObstacle(int x, int y)
{
    // store obstacles in a simple vector; check for duplicates
    for (int i = 0; i < obstacles.size(); ++i)
    {
        if (obstacles[i][0] == x && obstacles[i][1] == y)
            return false;
    }

    obstacles.push_back(new int[2]{x, y});
    return true;
}