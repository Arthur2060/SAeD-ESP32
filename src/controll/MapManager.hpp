#include <vector>
#include "Demarcacao.hpp"
#include "PathCalc.hpp"

class MapManager
{
private:
    std::vector<std::vector<bool>> map;
    float distanciaDeQuadro = 0.3f;
    Demarcacao demarcacao;
    PathCalc pathCalc;

public:
    MapManager() {}

    MapManager(int scaleX, int scaleY, float distanciaDeQuadro)
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
        this->demarcacao = Demarcacao(map);
        this->pathCalc = PathCalc(map);
    }

    MapManager(std::vector<std::vector<bool>> map, float distanciaDeQuadro)
    {
        this->map = map;
        this->distanciaDeQuadro = distanciaDeQuadro;
        this->demarcacao = Demarcacao(map);
    }

    float getDistanciaDeQuadro();
    void setDistanciaDeQuadro(float distancia);

    bool addObstacle(float distanceX, float distanceY);

    Demarcacao getDemarcacao();

    std::vector<std::vector<bool>> getMap();
    
    void setNewMap(std::vector<std::vector<bool>> map);
    void setNewMap(int scalex, int scaley);
    void setNewMap(int *scale);

    std::vector<char> setTarget(int x, int y);
    std::vector<char> setTarget(int *coordinates);
};

float MapManager::getDistanciaDeQuadro()
{
    return distanciaDeQuadro;
}

void MapManager::setDistanciaDeQuadro(float distancia)
{
    if (distancia > 0)
    {
        distanciaDeQuadro = distancia;
    }
}

bool MapManager::addObstacle(float distanceX, float distanceY)
{
    float x = distanceX / distanciaDeQuadro;
    float y = distanceY / distanciaDeQuadro;

    if (!map[x][y])
    {
        map[x][y] = !map[x][y];
        return true;
    }

    return false;
}

Demarcacao MapManager::getDemarcacao()
{
    return this->demarcacao;
}

std::vector<std::vector<bool>> MapManager::getMap()
{
    return this->map;
}

void MapManager::setNewMap(std::vector<std::vector<bool>> map)
{
    this->map = map;
    this->demarcacao.setMap(this->map);
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

    this->demarcacao.setMap(this->map);
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

    this->demarcacao.setMap(this->map);
}

std::vector<char> MapManager::setTarget(int x, int y)
{
    return pathCalc.setTarget(x, y);
}

std::vector<char> MapManager::setTarget(int *coordinates)
{
    return pathCalc.setTarget(coordinates);
}