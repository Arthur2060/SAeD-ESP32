#ifndef SAED_H
#define SAED_H

#include "model/ColorDetect.h"
#include "model/Claw.h"

#include "controll/Motores.h"
#include "controll/MapManager.h"
#include "controll/PathCalc.h"
#include "controll/Demarcacao.h"
#include "controll/Radar.h"

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
        MapManager *mapManager;
        Motores *motores;
        ColorDetect *colorDetect;
        Demarcacao *demarcacao;
        PathCalc *pathCalc;
        Radar *radar;
        Claw *claw;

    public:
        Principal(float cellScale);

        void begin();

        void principalLoop();
    };
}

#endif