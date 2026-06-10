#ifndef SAED_H
#define SAED_H

#include "ColorDetect.h"
#include "Claw.h"

#include "Motores.h"
#include "MapManager.h"
#include "Demarcacao.h"
#include "Radar.h"

#include "BluetoothSerial.h"
#include "ArduinoJson.h"

#include <vector>
#include <random>
#include <string>

namespace SAeD
{
    class Principal
    {
    private:
        const int MAX_NO_OBSTACLE_LIMIT = 3;
        int noObstacleLimit = MAX_NO_OBSTACLE_LIMIT;

        void newItemLoopPrimary();
        void mapLoopPrimary();
        void dispatchLoopPrimary();

        void received();
        void stock(area area);
        void move(int *end);
        void analise();

        BluetoothSerial BTS;
        MapManager mapManager;
        Demarcacao demarcacao;
        Motores motores;
        ColorDetect colorDetect;
        Radar radar;
        Claw claw;

    public:
        Principal(float cellScale);

        void begin();

        void principalLoop();
    };
}

#endif
