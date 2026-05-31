#include "Principal.h"

using namespace SAeD;
using namespace std;

void Principal::begin()
{
    this->BTS.begin("SAeD");
    this->motores.begin();
    this->radar.begin();
    this->colorDetect.begin();
}

void Principal::principalLoop()
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

            int random[2] = {distr(gen), distr(gen)};
            int* current = mapManager.getCurrentCell();

            this->pathCalc.createPath(current, random);
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

    if (BTS.available() > 0) {
        JsonDocument payload;
        deserializeJson(payload, BTS.readString());

        String route = payload["route"];
        String method = payload["method"];
        String body = payload["body"];

        if (route == "controll") {
            if (method == "Front") {
                motores.lerComandos({'W'});
            } else if (method == "Bottom") {
                motores.lerComandos({'S'});
            } else if (method == "Left") {
                motores.lerComandos({'A'});
            } else if (method == "Right") {
                motores.lerComandos({'D'});
            } else if (method == "Spin") {
                motores.lerComandos({'R'});
            }
        }

        serializeJson(payload, Serial);
    }
}

void Principal::secondaryLoop()
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

void Principal::received()
{
    std::vector<char> path = pathCalc.createPath(mapManager.getCurrentCell(), demarcacao.recieveingCell);
    newItemState = SAeDTransitionNewItem[newItemState];
    bool resp = motores.lerComandos(path);

    if (resp)
    {
        newItemState = SAeDTransitionNewItem[newItemState];
    }
}

area Principal::analise()
{
    for (int c = 0; c < demarcacao.areas.size(); c++)
    {
        area target = this->demarcacao.areas[c];

        if (this->colorDetect.isThisColor(target.areaColor))
        {
            int *startCell = demarcacao.areas[c].startCell;

            startCell[0] -= 1;
            startCell[1] += 1;

            pathCalc.createPath(mapManager.getCurrentCell(), startCell);
            newItemState = SAeDTransitionNewItem[newItemState];
            return target;
        }
    }
    newItemState = SAeDTransitionNewItem[newItemState];
    newItemState = SAeDTransitionNewItem[newItemState];
}

void Principal::setNewMap(int *dimensions, int *initial)
{
    mapManager.setNewMap(dimensions);
    mapManager.setCurrentCell(initial[0], initial[1]);

    mapState = SAeDStateMap::Mapping;
}