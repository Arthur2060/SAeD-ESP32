#include "Principal.h"

using namespace SAeD;
using namespace std;

Principal::Principal(float cellScale)
{
    this->motores = new Motores();
    this->radar = new Radar();
    this->colorDetect = new ColorDetect();
    this->claw = new Claw();
}
void Principal::begin()
{
    BTS.begin("SAeD");
    motores->begin();
    radar->begin();
    colorDetect->begin();
    claw->begin();
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
                int target[2] = {mapManager->currentCell[0], (mapManager->currentCell[1] + 1)};

                move(target);
            }
            else if (method == "Bottom")
            {
                int target[2] = {mapManager->currentCell[0], (mapManager->currentCell[1] - 1)};

                move(target);
            }
            else if (method == "Left")
            {
                motores->lerComandos({'A'});
            }
            else if (method == "Right")
            {
                motores->lerComandos({'D'});
            }
            else if (method == "Spin")
            {
                motores->lerComandos({'R'});
            }
            else if (method == "Start")
            {
                this->move(demarcacao->startCell);
            }
            else if (method == "GET")
            {
            }
            else if (method == "PUT")
            {
                delay(3000);
            }
        }
        else if (route == "area")
        {
            if (method == "GET")
            {
                JsonDocument doc;
                JsonArray array = doc.to<JsonArray>();
                for (int c = 0; c <= demarcacao->areas.size(); c++)
                {
                    JsonDocument sub;

                    sub["name"] = demarcacao->areas[c].name;
                    sub["startCellX"] = demarcacao->areas[c].startCell[0];
                    sub["startCellY"] = demarcacao->areas[c].startCell[1];
                    sub["endCellX"] = demarcacao->areas[c].endCell[0];
                    sub["endCellY"] = demarcacao->areas[c].endCell[1];

                    array.add(sub);
                }

                serializeJson(doc, BTS);
            }
            else if (method == "POST")
            {
                JsonDocument doc;
                deserializeJson(doc, body);

                string name = doc["name"];

                demarcacao->setNewArea(
                    {doc["initialX"], doc["initialY"]},
                    {doc["finalX"], doc["finalY"]},
                    colorDetect->defineColor(),
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

                // map PUT received but MapManager no longer holds full map data; ignoring
                (void)doc;
            }
        }
        else if (route == "receiving")
        {
            if (method == "GET")
            {
                JsonDocument doc;
                doc["receivingX"] = demarcacao->recieveingCell[0];
                doc["receivingY"] = demarcacao->recieveingCell[1];

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

                demarcacao->recieveingCell[0] = doc["x"];
                demarcacao->recieveingCell[1] = doc["y"];
            }
        }
    }
}

void Principal::received()
{
    move(demarcacao->recieveingCell);
}

void Principal::analise()
{
    for (int c = 0; c < demarcacao->areas.size(); c++)
    {
        area target = demarcacao->areas[c];

        if (colorDetect->isThisColor(target.areaColor))
        {
            int *startCell = demarcacao->areas[c].startCell;

            startCell[0] -= 1;
            startCell[1] += 1;

            stock(target);
        }
    }
    (void)0;
}

void Principal::stock(area area)
{
    int target[2] = {area.startCell[0] - 1, area.startCell[1] - 1};

    move(target);
}

void Principal::newItemLoopPrimary() {}
void Principal::mapLoopPrimary() {}
void Principal::dispatchLoopPrimary() {}

void Principal::move(int *end)
{
    // MapManager no longer holds a full map; assume pathCalc handles validity
    vector<char> path = pathCalc->createPath(mapManager->currentCell, end);
    motores->lerComandos(path);
    mapManager->currentCell[0] = end[0];
    mapManager->currentCell[1] = end[1];
}