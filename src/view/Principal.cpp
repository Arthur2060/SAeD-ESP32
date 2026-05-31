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
    void newItemLoopPrimary();
    void mapLoopPrimary();
    void dispatchLoopPrimary();

    if (BTS.available() > 0)
    {
        JsonDocument payload;
        deserializeJson(payload, BTS.readString());

        String route = payload["route"];
        String method = payload["method"];
        String body = payload["body"];

        if (route == "controll")
        {
            if (method == "Front")
            {
                motores.lerComandos({'W'});
            }
            else if (method == "Bottom")
            {
                motores.lerComandos({'S'});
            }
            else if (method == "Left")
            {
                motores.lerComandos({'A'});
            }
            else if (method == "Right")
            {
                motores.lerComandos({'D'});
            }
            else if (method == "Spin")
            {
                motores.lerComandos({'R'});
            }
        }
        else if (route == "area")
        {
            if (method == "GET")
            {
                JsonDocument doc;
                doc["areas"] = demarcacao.areas;

                serializeJson(doc, BTS);
            }
            else if (method == "POST")
            {
                JsonDocument doc;
                deserializeJson(doc, body);

                demarcacao.setNewArea(
                    {doc["initialX"], doc["initialY"]},
                    {doc["finalX"], doc["finalY"]},
                    colorDetect.defineColor(),
                    doc["name"]);
            }
        }
        else if (route == "map")
        {
            if (method == "GET")
            {
                JsonDocument doc;
                doc["map"] = mapManager.getMap();

                serializeJson(doc, BTS);
            }
            else if (method == "PUT")
            {
                JsonDocument doc;
                deserializeJson(doc, body);

                mapManager.setNewMap(doc["scaleX"], doc["scaleY"]);
            }
        }
        else if (route == "receiving")
        {
            if (method == "GET")
            {
                JsonDocument doc;
                doc["receivingX"] = demarcacao.recieveingCell[0];
                doc["receivingY"] = demarcacao.recieveingCell[1];

                serializeJson(doc, BTS);
            }
            else if (method == "PUT")
            {
                JsonDocument doc;
                deserializeJson(doc, body);

                demarcacao.recieveingCell[0] = doc["x"];
                demarcacao.recieveingCell[1] = doc["y"];
            }
        }

        serializeJson(payload, Serial);
    }
}

void Principal::secondaryLoop()
{
    void newItemLoopSecondary();
    void mapLoopSecondary();
    void dispatchLoopSecondary();
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

void Principal::newItemLoopPrimary()
{
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
void Principal::mapLoopPrimary()
{
    switch (mapState)
    {
    case SAeDStateMap::Wait:
        break;
    case SAeDStateMap::Mapping:
        obstacle = radar.getObstacle();

        if (obstacle[0] == 0 && obstacle[1] == 0)
            return;
        (!mapManager.addObstacle(obstacle[0], obstacle[1])) ? noObstacleLimit -= 1 : noObstacleLimit = MAX_NO_OBSTACLE_LIMIT;
        break;
    case SAeDStateMap::Demarc:
        break;
    }
}
void Principal::dispatchLoopPrimary()
{
    switch (dispatchState)
    {
    case SAeDStateDispatch::Wait:
        break;
    case SAeDStateDispatch::GetFromStock:
        break;
    case SAeDStateDispatch::Dispatch:
        break;
    }
}

void Principal::newItemLoopSecondary()
{

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
void Principal::mapLoopSecondary()
{
    switch (mapState)
    {
    case SAeDStateMap::Wait:
        break;
    case SAeDStateMap::Mapping:
        break;
    case SAeDStateMap::Demarc:
        break;
    }
}
void Principal::dispatchLoopSecondary()
{

    switch (dispatchState)
    {
    case SAeDStateDispatch::Wait:
        break;
    case SAeDStateDispatch::GetFromStock:
        break;
    case SAeDStateDispatch::Dispatch:
        break;
    }
}