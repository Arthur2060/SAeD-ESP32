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
#include <map>

namespace SAeD
{
    enum class SAeDStateMap
    {
        Wait,
        Mapping,
        Demarc
    };

    enum class SAeDStateDispatch
    {
        Wait,
        GetFromStock,
        Dispatch
    };

    enum class SAeDStateNewItem
    {
        Wait,
        GetNew,
        Analise,
        Stock
    };

    class Principal
    {
    private:
        std::vector<char> spinCommand = {'R', 'R', 'R'};

        std::map<SAeDStateMap, SAeDStateMap> SAeDTransitionMap = {
            {SAeDStateMap::Wait, SAeDStateMap::Mapping},
            {SAeDStateMap::Mapping, SAeDStateMap::Demarc},
            {SAeDStateMap::Demarc, SAeDStateMap::Wait}};

        std::map<SAeDStateDispatch, SAeDStateDispatch> SAeDTransitionDispatch = {
            {SAeDStateDispatch::Wait, SAeDStateDispatch::GetFromStock},
            {SAeDStateDispatch::GetFromStock, SAeDStateDispatch::Dispatch},
            {SAeDStateDispatch::Dispatch, SAeDStateDispatch::Wait}};

        std::map<SAeDStateNewItem, SAeDStateNewItem> SAeDTransitionNewItem = {
            {SAeDStateNewItem::Wait, SAeDStateNewItem::GetNew},
            {SAeDStateNewItem::GetNew, SAeDStateNewItem::Analise},
            {SAeDStateNewItem::Analise, SAeDStateNewItem::Stock},
            {SAeDStateNewItem::Stock, SAeDStateNewItem::Wait}};

        SAeDStateDispatch dispatchState = SAeDStateDispatch::Wait;
        SAeDStateMap mapState = SAeDStateMap::Wait;
        SAeDStateNewItem newItemState = SAeDStateNewItem::Wait;

        const int MAX_NO_OBSTACLE_LIMIT = 3;
        int noObstacleLimit = MAX_NO_OBSTACLE_LIMIT;

        double currentOdometri;

        std::vector<double> obstacle = {};

        void newItemLoopPrimary();
        void mapLoopPrimary();
        void dispatchLoopPrimary();

        void newItemLoopSecondary();
        void mapLoopSecondary();
        void dispatchLoopSecondary();

        void setNewMap(int *dimensions, int *initial);
        void received();
        void stock(area area);
        void move(int *end);
        void analise();

        BluetoothSerial BTS;
        MapManager mapManager;
        Motores motores;
        ColorDetect colorDetect;
        Demarcacao demarcacao;
        PathCalc pathCalc;
        Radar radar;
        Claw claw;

    public:
        Principal(int scaleX, int scaleY, float cellScale);

        void begin();

        void principalLoop();
        void secondaryLoop();
    };
}

#endif