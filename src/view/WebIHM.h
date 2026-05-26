#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "ArduinoJson.h"
#include "SAeD.h"
#include <WiFi.h>

AsyncWebServer server(80);

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

        defineRoutes();

        server.begin();
    }

    // Retorna o IP do servidor WebSocket
    IPAddress getIp()
    {
        return this->ip;
    }

    // Envia algum contêudo no formato JSON para todos os clientes conectados
    String sendSomething(JsonDocument content)
    {
        String payload;

        serializeJson(content, payload);

        return payload;
    }

    void defineRoutes()
    {
        server.on("/map", HTTP_GET, [this](AsyncWebServerRequest *request) {

            std::vector<std::vector<bool>> map = core.getMap();
    
            JsonDocument doc;

            doc["scaleX"] = map.size();
            doc["scaleY"] = map[0].size();
    
            request->send(200, "text/html", sendSomething(doc)); });

        server.on("/received", HTTP_POST, [this](AsyncWebServerRequest *request) {
            
        });

        server.on("/target", HTTP_GET, [this](AsyncWebServerRequest *request) {
            std::vector<int> position = core.getTargetPosition();

            JsonDocument doc;

            doc["x"] = position[0];
            doc["y"] = position[1];

            request->send(200, "application/json", sendSomething(doc)); 
        });

        server.on("/current", HTTP_GET, [this](AsyncWebServerRequest *request) {
            std::vector<int> position = core.getCurrentPosition();

            JsonDocument doc;

            doc["x"] = position[0];
            doc["y"] = position[1];

            request->send(200, "application/json", sendSomething(doc)); 
        });
    }
};