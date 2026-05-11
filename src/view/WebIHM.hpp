#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "ArduinoJson.h"
#include "SAeD.hpp"
#include <WiFi.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

class WebIHM
{
private:
    SAeD core;
    IPAddress ip;

public:
    WebIHM() {}

    void begin(char *ssid, char *password)
    {
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(300);
        }

        ip = WiFi.localIP();

        server.addHandler(&ws);

        defineRoutes();

        server.begin();
    }

    // Retorna o IP do servidor WebSocket
    IPAddress getIp()
    {
        return this->ip;
    }

    // Envia algum contêudo no formato JSON para todos os clientes conectados
    void sendSomething(JsonDocument content)
    {
        String payload;

        serializeJson(content, payload);

        ws.textAll(payload);
    }

    // Envia o contêudo do mapa atual
    void enviarMapa()
    {
        std::vector<std::vector<bool>> map = core.getMap();

        JsonDocument content;
        JsonArray mapJson = content.to<JsonArray>();

        for (int c = 0; c <= sizeof(map); c++)
        {
            JsonArray targetLine = mapJson.add<JsonArray>();
            for (int d = 0; d <= sizeof(map[c]); d++)
            {
                targetLine.add(map[c][d]);
            }
        }

        sendSomething(content);
    }

    void defineRoutes()
    {
        server.on("/target", HTTP_POST, [this](AsyncWebServerRequest *request)
                  {
                int target[2] = {request->getAttribute("x", 0.0), request->getAttribute("y", 0.0)};
                
                std::vector<char> path = core.setTarget(target);
                JsonDocument doc;
                JsonArray pathJson = doc.to<JsonArray>();

                for (char step : path) {
                    pathJson.add(step);
                }
                
                sendSomething(doc); });

        server.on("/map", HTTP_POST, [this](AsyncWebServerRequest *request)
                  {
                      int dimensions[2] = {request->getAttribute("scaleX", 0.0), request->getAttribute("scaleY", 0.0)};
                      int initial[2] = {request->getAttribute("initialX", 0.0), request->getAttribute("initialY", 0.0)};

                      core.setNewMap(dimensions, initial); });
    }
};