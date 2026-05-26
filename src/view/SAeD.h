#include "model/Color.hpp"

#include "controll/Motores.h"
#include "controll/MapManager.hpp"
#include "controll/SAeDStateMachine.hpp"
#include "controll/Radar.hpp"

#include <vector>
#include <random>
#include <string>

std::vector<char> spinCommand = {'R', 'R', 'R'};

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
    SAeD() { 
        this->mapManager = MapManager(10, 10, 0.3);

        this->mapManager.getDemarcacao().setNewArea({9, 9}, {10, 10}, {255, 255, 255}, "white");
    }
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

    void setNewMap(int *dimensions, int *initial);
    void principalLoop();
    void secondaryLoop();
    void received();
    void analise();

    std::vector<std::vector<bool>> getMap() { return mapManager.getMap(); }
    
    std::vector<char> setTarget(int *target);
    std::vector<int> getCurrentPosition();
    std::vector<int> getTargetPosition();
};

void SAeD::principalLoop()
    {
        switch (mapState)
        {
        case SAeDStateMap::Wait:
            break;
        case SAeDStateMap::Mapping:
            if (noObstacleLimit <= 0)
            {
                motores.lerComandos(spinCommand);
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

void SAeD::secondaryLoop()
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

void SAeD::received() {
    std::vector<char> path = setTarget(mapManager.getDemarcacao().recieveingCell);
    newItemState = SAeDTransitionNewItem[newItemState];
    bool resp = motores.lerComandos(path);

    if (resp) {
        newItemState = SAeDTransitionNewItem[newItemState];
    }
}

void SAeD::analise() {
    std::vector<uint> cor = color.detectaCor();

    for (int c = 0 ; c < mapManager.getDemarcacao().areas.size() ; c++) {
        if (
            cor[0] == mapManager.getDemarcacao().areas[c].color[0] &&
            cor[1] == mapManager.getDemarcacao().areas[c].color[1] &&
            cor[2] == mapManager.getDemarcacao().areas[c].color[2])
        {
            int* startCell = mapManager.getDemarcacao().areas[c].startCell;

            mapManager.setTarget(startCell[0] - 1, startCell[1] + 1);
            newItemState = SAeDTransitionNewItem[newItemState];
            return;
        }
    }
    newItemState = SAeDTransitionNewItem[newItemState];
    newItemState = SAeDTransitionNewItem[newItemState];
}

std::vector<char> SAeD::setTarget(int *target)
{
    return mapManager.setTarget(target);
}

void SAeD::setNewMap(int *dimensions, int *initial)
{
    mapManager.setNewMap(dimensions);
    mapManager.setCurrentCell(initial[0], initial[1]);
    
    mapState = SAeDStateMap::Mapping;
}

std::vector<int> SAeD::getCurrentPosition()
{
    return mapManager.getCurrentCell();
}

std::vector<int> SAeD::getTargetPosition()
{
    return mapManager.getTarget();
}