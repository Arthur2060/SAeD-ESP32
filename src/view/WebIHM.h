#ifndef WEB_IHM_H
#define WEB_IHM_H

#include "ArduinoJson.h"
#include <BluetoothSerial.h>
#include "SAeD.h"

namespace N
{
    class WebIHM
    {
    private:
        SAeD core;
        BluetoothSerial BTS;
        const char *DEFAULT_SSID = "SAeD - SENAI";

    public:
        WebIHM();
        void begin(char *ssid);

        // Envia algum contêudo no formato JSON para todos os clientes conectados
        String sendSomething(JsonDocument content);

        void defineRoutes();
    };
}

#endif