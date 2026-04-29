#include "model/Motores.hpp"
#include "model/Color.hpp"

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
    SAeDStateMap mapState = SAeDStateMap::Mapping;
    SAeDStateNewItem newItemState = SAeDStateNewItem::Wait;

    const int MAX_NO_OBSTACLE_LIMIT = 3;
    int noObstacleLimit = MAX_NO_OBSTACLE_LIMIT;

    float currentOdometri;

    std::vector<float> obstacle = {};

public:
    SAeD() { this->mapManager = MapManager(10, 10, 0.3); }

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
                motores.executarGiro(360);
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
        switch (mapState)
        {
        case SAeDStateMap::Wait:
            break;
        case SAeDStateMap::Mapping:
            currentOdometri = motores.atualizarOdometria();

            obstacle = radar.getObstacle(currentOdometri);

            const bool resul = mapManager.addObstacle(obstacle[0], obstacle[1]);
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
};