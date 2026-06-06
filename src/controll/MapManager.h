#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <vector>

using namespace std;

namespace SAeD
{
    class MapManager
    {
    private:
        float distanciaDeQuadro = 0.3f;

    public:
        vector<int *> obstacles;
        int currentCell[2];

        MapManager();
        MapManager(float distanciaDeQuadro);

        float getDistanciaDeQuadro();
        float setDistanciaDeQuadro(float distancia);

        vector<int *> getObstacles();

        bool addObstacle(float distanceX, float distanceY);
    };
}

#endif