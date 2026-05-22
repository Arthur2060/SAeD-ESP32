#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "ArduinoJson.h"
#include "SAeD.hpp"
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

    // Envia o contêudo do mapa atual

    void defineRoutes()
    {
        server.on("/map", HTTP_GET, [this](AsyncWebServerRequest *request)
                  {

            std::vector<std::vector<bool>> map = core.getMap();
    
            JsonDocument doc;

            doc["scaleX"] = map.size();
            doc["scaleY"] = map[0].size();
    
            request->send(200, "text/html", sendSomething(doc)); });

        server.on("/map", HTTP_POST, [this](AsyncWebServerRequest *request)
                  {
                    int dimensions[2] = {};


                    if (request->hasParam("scaleX")){
                        dimensions[0] = request->getParam("scaleX")->value().toInt();
                    }

                    if (request->hasParam("scaleY")){
                        dimensions[1] = request->getParam("scaleY")->value().toInt();
                    }

                    int initial[2] = {};

                    if (request->hasParam("initialX")){
                        initial[0] = request->getParam("initialX")->value().toInt();
                    }

                    if (request->hasParam("initialY")){
                        initial[1] = request->getParam("initialY")->value().toInt();
                    }
            
                    core.setNewMap(dimensions, initial); 
                    
                    request->send(201, "application/json", "Mapa criado com exito!"); });

        server.on("/target", HTTP_GET, [this](AsyncWebServerRequest *request)
                  {
            JsonDocument doc;
            std::vector<int> target = core.getTargetPosition();

            doc["x"] = target[0];
            doc["y"] = target[1];
            
            request->send(200, "application/json", sendSomething(doc)); });

        server.on("/target", HTTP_PUT, [this](AsyncWebServerRequest *request)
                  {
                      JsonDocument doc;

                      int target[2] = {2, 5};
                
                        std::vector<char> path = core.setTarget(target);
                        doc["path"] = path;

                
                    request->send(200, "application/json", sendSomething(doc)); });

        server.on("/current", HTTP_GET, [this](AsyncWebServerRequest *request)
                  {

            std::vector<int> position = core.getCurrentPosition();

            JsonDocument doc;

            doc["x"] = position[0];
            doc["y"] = position[1];

            request->send(200, "application/json", sendSomething(doc)); });
    }
};