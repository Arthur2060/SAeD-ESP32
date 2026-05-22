#include "model/Color.hpp"

#include "controll/Motores.hpp"
#include "controll/MapManager.hpp"
#include "controll/SAeDStateMachine.hpp"
#include "controll/Radar.hpp"

#include <vector>
#include <random>

class SAeD
{
private:
    Motores motores;
    Color color;
    Radar radar;

    MapManager mapManager;

    SAeDStateDispatch dispatchState = SAeDStateDispatch::Wait;
    SAeDStateMap mapState = SAeDStateMap::Wait;
    SAeDStateNewItem newItemState = SAeDStateNewItem::Wait;

    const int MAX_NO_OBSTACLE_LIMIT = 3;
    int noObstacleLimit = MAX_NO_OBSTACLE_LIMIT;

    double currentOdometri;

    std::vector<double> obstacle = {};

public:
    SAeD() { this->mapManager = MapManager(10, 10, 0.3); }
    SAeD(int scaleX, int scaleY, float cellScale)
    {
        this->mapManager = MapManager(scaleX, scaleY, cellScale);
    }

    void begin()
    {
        motores.begin();
        radar.begin();
        color.begin();
    }

    void principalLoop()
    {
        switch (mapState)
        {
        case SAeDStateMap::Wait:
            break;
        case SAeDStateMap::Mapping:
            if (noObstacleLimit <= 0)
            {
                motores.lerComandos("RRR");
            }
            else
            {
                std::random_device rd;
                std::mt19937 gen(rd());

                std::uniform_int_distribution<> distr(1, sizeof(mapManager.getMap()));

                int randomX = distr(gen);
                int randomY = distr(gen);

                mapManager.setTarget(randomX, randomY);
                noObstacleLimit = MAX_NO_OBSTACLE_LIMIT;
            }
            break;
        case SAeDStateMap::Demarc:
            break;
        }

        switch (dispatchState)
        {
        case SAeDStateDispatch::Wait:
            break;
        case SAeDStateDispatch::GetFromStock:
            break;
        case SAeDStateDispatch::Dispatch:
            break;
        }

        switch (newItemState)
        {
        case SAeDStateNewItem::Wait:
            break;
        case SAeDStateNewItem::GetNew:
            break;
        case SAeDStateNewItem::Analise:
            break;
        case SAeDStateNewItem::Stock:
            break;
        }
    }

    void secondaryLoop()
    {
        bool resul = false;
        switch (mapState)
        {
        case SAeDStateMap::Wait:
            break;
        case SAeDStateMap::Mapping:
            obstacle = radar.getObstacle();

            if (obstacle[0] == 0 && obstacle[1] == 0)
                return;
            resul = mapManager.addObstacle(obstacle[0], obstacle[1]);
            (!resul) ? noObstacleLimit -= 1 : noObstacleLimit = MAX_NO_OBSTACLE_LIMIT;
            obstacle = {};
            break;
        case SAeDStateMap::Demarc:
            break;
        }

        switch (dispatchState)
        {
        case SAeDStateDispatch::Wait:
            break;
        case SAeDStateDispatch::GetFromStock:
            break;
        case SAeDStateDispatch::Dispatch:
            break;
        }

        switch (newItemState)
        {
        case SAeDStateNewItem::Wait:
            break;
        case SAeDStateNewItem::GetNew:
            break;
        case SAeDStateNewItem::Analise:
            break;
        case SAeDStateNewItem::Stock:
            break;
        }
    }

    std::vector<std::vector<bool>> getMap() { return mapManager.getMap(); }

    std::vector<char> setTarget(int *target)
    {
        newItemState = SAeDStateNewItem::GetNew;
        return mapManager.setTarget(target);
    }

    void setNewMap(int *dimensions, int *initial)
    {
        mapManager.setNewMap(dimensions);
        mapManager.setCurrentCell(initial[0], initial[1]);

        mapState = SAeDStateMap::Mapping;
    }

    std::vector<int> getCurrentPosition()
    {
        return mapManager.getCurrentCell();
    }
};