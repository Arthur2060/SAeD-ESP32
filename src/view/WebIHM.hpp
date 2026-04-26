#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "ArduinoJson.h"
#include "controll/MapManager.hpp"
#include <WiFi.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

class WebIHM {
    private:
        MapManager mapManager;
        IPAddress ip;

    public:
        WebIHM(MapManager MapManager) {
            this->mapManager = mapManager;
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
};