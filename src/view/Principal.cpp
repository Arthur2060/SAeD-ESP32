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

    JsonDocument log;

    log["route"] = "log";
    log["method"] = "POST";
    log["body"] = "( distance; 1 - angle; 1 )";

    serializeJson(log, BTS);
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
    // MapManager no longer holds a full map; assume pathCalc handles validity
    vector<char> path = mapManager.createPath(end);
    motores.lerComandos(path);
    mapManager.currentCell[0] = end[0];
    mapManager.currentCell[1] = end[1];
}