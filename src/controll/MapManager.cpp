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


vector<char> MapManager::createPath(int *targetCell)
{
    int diference[2] = {currentCell[0] - targetCell[0], currentCell[1] - targetCell[1]};
    vector<char> path;

    if (diference[1] < 0)
    {
        for (int c2 = 0; c2 <= diference[0]; c2++)
        {
            path.push_back('W');
        }
    }
    else
    {
        path.push_back('S');
        for (int c2 = diference[0]; c2 < 0; c2++)
        {
            path.push_back('W');
        }
    }
    if (diference[0] > 0)
    {
        path.push_back('D');
        for (int c2 = 0; c2 <= diference[0]; c2++)
        {
            path.push_back('W');
        }
    }
    else if (diference[0] < 0)
    {
        path.push_back('A');
        for (int c2 = diference[0]; c2 <= 0; c2++)
        {
            path.push_back('W');
        }
    }

    return path;
}

void MapManager::setCellScale(float cellScale) {
    this->cellScale = cellScale;
}