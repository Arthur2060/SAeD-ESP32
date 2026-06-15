#include "Principal.h"

using namespace SAeD;
using namespace std;

Principal::Principal(float cellScale)
{
    this->mapManager.setCellScale(cellScale);
}
void Principal::begin()
{
    BTS.begin("SAeD");
    radar.begin();
    motores.begin();
    colorDetect.begin();
    claw.idle();
}

void Principal::principalLoop()
{
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
                char command[1] = {'A'};
                motores.lerComandos(command);
            }
            else if (method == "Right")
            {
                char command[1] = {'D'};
                motores.lerComandos(command);
            }
            else if (method == "Spin")
            {
                char command[1] = {'R'};
                motores.lerComandos(command);
            }
            else if (method == "Start")
            {
                move(demarcacao.startCell);
            }
            else if (method == "GET")
            {
                claw.get();
            }
            else if (method == "PUT")
            {
                claw.put();
            }
            else if (method == "IDLE")
            {
                claw.idle();
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

void Principal::received()
{
    move(demarcacao.recieveingCell);
    analise();
}

void Principal::analise()
{
    claw.get();
    delay(8000);
    area target;
    for (int c = 0; c < demarcacao.areas.size(); c++)
    {
        area target = demarcacao.areas[c];

        if (colorDetect.isThisColor(target.areaColor))
        {
            area target = demarcacao.areas[c];
        }
    };
    delay(12000);
    stock(target);
}

void Principal::stock(area area)
{
    int target[2] = {area.startCell[0] - 1, area.startCell[1] - 1};

    move(target);
    claw.put();
}

void Principal::newItemLoopPrimary() {}
void Principal::mapLoopPrimary() {}
void Principal::dispatchLoopPrimary() {}

void Principal::move(int *end)
{
    char* path = mapManager.createPath(end);
    motores.lerComandos(path);
    mapManager.currentCell[0] = end[0];
    mapManager.currentCell[1] = end[1];
    delete end;
}