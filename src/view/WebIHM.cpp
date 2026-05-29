#include "WebIHM.h"

using namespace N;
using namespace std;

WebIHM::WebIHM()
{
    BTS.begin(DEFAULT_SSID);
}

void WebIHM::begin(char *ssid)
{
    BTS.begin(ssid);
}

// Envia algum contêudo no formato JSON para todos os clientes conectados
String WebIHM::sendSomething(JsonDocument content)
{
    String payload;

    serializeJson(content, payload);

    return payload;
}

void WebIHM::defineRoutes()
{
    if (BTS.available() > 0)
    {
        JsonDocument payload;
        deserializeJson(payload, BTS.readString());

        String route = payload["route"];
        String method = payload["method"];
        String body = payload["body"];

        if (route == "map")
        {
            if (method == "GET")
            {
                std::vector<std::vector<bool>> map = core.getMap();

                JsonDocument doc;

                doc["scaleX"] = map.size();
                doc["scaleY"] = map[0].size();

                BTS.println(sendSomething(doc));
            } else if (method == "PUT") {
                JsonDocument requestBody;
                deserializeJson(requestBody, body);

                int scale[2] = {requestBody["scaleX"], requestBody["scaleY"]};
                int initial[2] = {requestBody["initialX"], requestBody["initialY"]};

                core.setNewMap(scale, initial);
            }
        }
        else if (route == "target")
        {
            if (method == "GET")
            {
                std::vector<int> position = core.getTargetPosition();

                JsonDocument doc;

                doc["x"] = position[0];
                doc["y"] = position[1];

                BTS.println(sendSomething(doc));
            }
        }
        else if (route == "position")
        {
            if (method == "GET")
            {
                std::vector<int> position = core.getCurrentPosition();

                JsonDocument doc;

                doc["x"] = position[0];
                doc["y"] = position[1];

                BTS.println(sendSomething(doc));
            }
        }
        else if (route == "newItem")
        {
            if (method == "POST")
            {
                core.received();
            }
        }
    }
}