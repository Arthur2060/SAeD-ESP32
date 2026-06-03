#include "Principal.h"

using namespace SAeD;
using namespace std;

Principal::Principal(int scaleX, int scaleY, float cellScale)
{
    int scale[2] = {scaleX, scaleY};
    mapManager.setNewMap(scale);
    mapManager.setDistanciaDeQuadro(cellScale);
    motores.setCellScale(cellScale);
}

void Principal::begin()
{
    BTS.begin("SAeD");
    motores.begin();
    radar.begin();
    colorDetect.begin();
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

        string route = payload["route"];
        string method = payload["method"];
        string body = payload["body"];

        if (route == "controll")
        {
            if (method == "Front")
            {
                int target[2] = {mapManager.currentCell[0], (mapManager.currentCell[1] + 1)};
                
                move(target);
            }
            else if (method == "Bottom")
            {
                int target[2] = {mapManager.currentCell[0], (mapManager.currentCell[1] - 1)};

                move(target);
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
            else if (method == "Start")
            {
                this->move(demarcacao.startCell);
            }
        }
        else if (route == "area")
        {
            if (method == "GET")
            {
                JsonDocument doc;
                JsonArray array = doc.to<JsonArray>();
                for (int c = 0; c <= demarcacao.areas.size(); c++)
                {
                    JsonDocument sub;

                    sub["name"] = demarcacao.areas[c].name;
                    sub["startCellX"] = demarcacao.areas[c].startCell[0];
                    sub["startCellY"] = demarcacao.areas[c].startCell[1];
                    sub["endCellX"] = demarcacao.areas[c].endCell[0];
                    sub["endCellY"] = demarcacao.areas[c].endCell[1];

                    array.add(sub);
                }

                serializeJson(doc, BTS);
            }
            else if (method == "POST")
            {
                JsonDocument doc;
                deserializeJson(doc, body);

                string name = doc["name"];

                demarcacao.setNewArea(
                    {doc["initialX"], doc["initialY"]},
                    {doc["finalX"], doc["finalY"]},
                    colorDetect.defineColor(),
                    name.c_str());
            }
        }
        else if (route == "map")
        {
            if (method == "GET")
            {
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
            else if (method == "POST")
            {
                received();
            }
            else if (method == "PUT")
            {
                JsonDocument doc;
                deserializeJson(doc, body);

                demarcacao.recieveingCell[0] = doc["x"];
                demarcacao.recieveingCell[1] = doc["y"];
            }
        }
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
    newItemState = SAeDTransitionNewItem[newItemState];
    move(demarcacao.recieveingCell);
    newItemState = SAeDTransitionNewItem[newItemState];
}

area Principal::analise()
{
    for (int c = 0; c < demarcacao.areas.size(); c++)
    {
        area target = demarcacao.areas[c];

        if (colorDetect.isThisColor(target.areaColor))
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
        analise();
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

void Principal::move(int* end)
{
    vector<char> path = pathCalc.createPath(mapManager.currentCell, end);
    motores.lerComandos(path);
    mapManager.currentCell[0] = end[0];
    mapManager.currentCell[1] = end[1];
}