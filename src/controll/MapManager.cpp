#include "MapManager.h"

using namespace SAeD;
using namespace std;

MapManager::MapManager() {
    for (int x = 0; x <= 5; x++)
    {
        map.push_back({});
        std::vector<bool> targetLine = map[x];
        for (int y = 0; y <= 5; y++)
        {
            map[x].push_back(false);
        }
    }

    this->distanciaDeQuadro = 0.3;
    this->currentCell[0] = 0;
    this->currentCell[1] = 0;
}

MapManager::MapManager(int scaleX, int scaleY, float distanciaDeQuadro)
{
    for (int x = 0; x <= scaleX; x++)
    {
        map.push_back({});
        std::vector<bool> targetLine = map[x];
        for (int y = 0; y <= scaleY; y++)
        {
            map[x].push_back(false);
        }
    }

    this->distanciaDeQuadro = distanciaDeQuadro;
    this->currentCell[0] = 0;
    this->currentCell[1] = 0;
}

MapManager::MapManager(std::vector<std::vector<bool>> map, float distanciaDeQuadro)
{
    this->map = map;
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

    if (!map[x][y])
    {
        map[x][y] = !map[x][y];
        return true;
    }

    return false;
}

std::vector<std::vector<bool>> MapManager::getMap()
{
    return this->map;
}

void MapManager::setNewMap(std::vector<std::vector<bool>> map)
{
    this->map = map;
}

void MapManager::setNewMap(int scalex, int scaley)
{
    for (int x = 0; x <= scalex; x++)
    {
        for (int y = 0; y <= scaley; y++)
        {
            this->map[x].push_back(false);
        }
    }
}

void MapManager::setNewMap(int *scale)
{
    for (int x = 0; x <= scale[0]; x++)
    {
        for (int y = 0; y <= scale[1]; y++)
        {
            this->map[x].push_back(false);
        }
    }
}