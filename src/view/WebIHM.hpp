#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "ArduinoJson.h"
#include "controll/MapManager.hpp"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

class WebIHM {
    private:
        MapManager mapManager;

    public:
        WebIHM(MapManager MapManager) {
            this->mapManager = mapManager;
        }

        void begin() {
            server.addHandler(&ws);
        }
};