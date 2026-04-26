#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "ArduinoJson.h"
#include "controll/MapManager.hpp"
#include "controll/PathCalc.hpp"
#include <WiFi.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

class WebIHM {
    private:
        MapManager mapManager;
        IPAddress ip;
        PathCalc pathCalc;

    public:
        WebIHM(MapManager MapManager) {
            this->mapManager = mapManager;
            this->pathCalc = PathCalc(mapManager.getMap());
        }

        void begin(char* ssid, char* password) {
            WiFi.begin(ssid, password);

            while (WiFi.status() != WL_CONNECTED) {
                delay(300);
            }


            ip = WiFi.localIP();

            server.addHandler(&ws);

            server.begin();
        }

        // Retorna o IP do servidor WebSocket
        IPAddress getIp() {
            return this->ip;
        }

        // Envia algum contêudo no formato JSON para todos os clientes conectados
        void sendSomething(JsonDocument content) {
            String payload;

            serializeJson(content, payload);

            ws.textAll(payload);
        }

        // Envia o contêudo do mapa atual
        void enviarMapa() {
            std::vector<std::vector<bool>> map = mapManager.getMap();

            JsonDocument content;
            JsonArray mapJson = content.to<JsonArray>();
            
            for (int c = 0 ; c <= sizeof(map) ; c++) {
                JsonArray targetLine = mapJson.add<JsonArray>();
                for (int d = 0 ; d <= sizeof(map[c]) ; d++) {
                    targetLine.add(map[c][d]);
                }
            }

            sendSomething(content);
        }

        void defineRoutes(AsyncWebServer server) {
            server.on("/path", HTTP_POST, [this](AsyncWebServerRequest* request) {
                int target[2] = {request->getAttribute("x", 0.0), request->getAttribute("y", 0.0)};

                std::vector<char> path = pathCalc.setTarget(target);
                JsonDocument doc;
                JsonArray pathJson = doc.to<JsonArray>();

                for (char step : path) {
                    pathJson.add(step);
                }

                sendSomething(doc);
            });
        }
};