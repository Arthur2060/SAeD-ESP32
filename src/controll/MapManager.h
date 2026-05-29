#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <vector>
#include "Demarcacao.h"
#include "PathCalc.h"

namespace N
{
    class MapManager
    {
    private:
        std::vector<std::vector<bool>> map;
        float distanciaDeQuadro = 0.3f;
        Demarcacao demarcacao;
        PathCalc pathCalc;

    public:
        MapManager() {}
        MapManager(int scaleX, int scaleY, float distanciaDeQuadro);
        MapManager(std::vector<std::vector<bool>> map, float distanciaDeQuadro);

        float getDistanciaDeQuadro();
        void setDistanciaDeQuadro(float distancia);

        bool addObstacle(float distanceX, float distanceY);

        Demarcacao getDemarcacao();

        std::vector<std::vector<bool>> getMap();

        void setNewMap(std::vector<std::vector<bool>> map);
        void setNewMap(int scalex, int scaley);
        void setNewMap(int *scale);
        void setCurrentCell(int x, int y);

        std::vector<char> setTarget(int x, int y);
        std::vector<char> setTarget(int *coordinates);
        std::vector<int> getCurrentCell();
        std::vector<int> getTarget();
    };
}

#endif