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
    for (int i = 0; i < obstacles.size(); ++i)
    {
        if (obstacles[i][0] == x && obstacles[i][1] == y)
            return false;
    }

    obstacles.push_back(new int[2]{x, y});
    return true;
}

char *MapManager::createPath(int *targetCell)
{
    int diference[2] = {currentCell[0] - targetCell[0], currentCell[1] - targetCell[1]};
    char path[diference[0] + diference[1]];

    if (diference[0] > 0)
    {
        for (int c2 = 0; c2 <= abs(diference[0]); c2++)
        {
            path[c2] = 'W';
        }
    }
    else
    {
        path[0] = 'S';
        for (int c2 = 1; c2 < abs(diference[0]) - 1; c2++)
        {
            path[c2] = 'W';
        }
    }
    if (diference[1] > 0)
    {
        path[diference[0]] = 'D';
        for (int c2 = 0; c2 <= abs(diference[1]); c2++)
        {
            path[c2] = 'W';
        }
    }
    else if (diference[1] < 0)
    {
        path[diference[0]] = 'A';
        for (int c2 = 1; c2 < abs(diference[0]) - 1; c2++)
        {
            path[c2] = 'W';
        }
    }

    delete targetCell;
    return path;
    delete[] path;
}

void MapManager::setCellScale(float cellScale)
{
    this->cellScale = cellScale;
}