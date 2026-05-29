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

        char route = payload["route"];
        char method = payload["method"];
        String body = payload["body"];

        switch (route)
        {
        case 'M':
            switch (method)
            {
            case 'G':
                std::vector<std::vector<bool>> map = core.getMap();

                JsonDocument doc;

                doc["scaleX"] = map.size();
                doc["scaleY"] = map[0].size();

                BTS.println(sendSomething(doc));
                break;
            }
            break;

        case 'T':
            switch (method)
            {
            case 'G':
                std::vector<int> position = core.getTargetPosition();

                JsonDocument doc;

                doc["x"] = position[0];
                doc["y"] = position[1];

                BTS.println(sendSomething(doc));
                break;
            }
            break;

        case 'P':
            switch (method)
            {
            case 'G':
                std::vector<int> position = core.getCurrentPosition();

                JsonDocument doc;

                doc["x"] = position[0];
                doc["y"] = position[1];

                BTS.println(sendSomething(doc));
                break;
            }
            break;

        case 'C':
            switch (method)
            {
            case 'P':
                core.received();
                break;
            }
        }
    }
}