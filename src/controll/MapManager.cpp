#include "MapManager.h"

using namespace SAeD;
using namespace std;

MapManager::MapManager()
{
    this->distanciaDeQuadro = 0.3;
    this->currentCell[0] = 0;
    this->currentCell[1] = 0;
}

MapManager::MapManager(float distanciaDeQuadro)
{
    this->distanciaDeQuadro = distanciaDeQuadro;
    this->currentCell[0] = 0;
    this->currentCell[1] = 0;
}

float MapManager::getDistanciaDeQuadro()
{
    return distanciaDeQuadro;
}

float MapManager::setDistanciaDeQuadro(float distancia)
{
    if (distancia > 0)
    {
        this->distanciaDeQuadro = distancia;
    }

    return this->distanciaDeQuadro;
}

bool MapManager::addObstacle(float distanceX, float distanceY)
{
    float x = this->currentCell[0] + (distanceX / distanciaDeQuadro);
    float y = this->currentCell[1] + (distanceY / distanciaDeQuadro);

    auto isNew = [this](int x, int y)
    {
        for (int c = 0; c < obstacles.size(); c++)
        {
            if (obstacles[c][0] == x && obstacles[c][1] == y)
            {
                return false;
            }
        }

        return true;
    };

    if (isNew(x, y))
    {
        obstacles.push_back(new int[2]{(int)x, (int)y});
        return true;
    }

    return false;
}

vector<int *> MapManager::getObstacles() {
    return obstacles;
}