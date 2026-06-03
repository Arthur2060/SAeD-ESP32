#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <vector>

namespace SAeD
{
    class MapManager
    {
    private:
        std::vector<std::vector<bool>> map;
        float distanciaDeQuadro = 0.3f;

    public:
        int currentCell[2];

        MapManager();
        MapManager(int scaleX, int scaleY, float distanciaDeQuadro);
        MapManager(std::vector<std::vector<bool>> map, float distanciaDeQuadro);

        float getDistanciaDeQuadro();
        float setDistanciaDeQuadro(float distancia);

        bool addObstacle(float distanceX, float distanceY);

        std::vector<std::vector<bool>> getMap();

        void setNewMap(std::vector<std::vector<bool>> map);
        void setNewMap(int scalex, int scaley);
        void setNewMap(int *scale);

        int *getCurrentCell();
        int *setCurrentCell(int x, int y);
    };
}

#endif