#ifndef SAED_H
#define SAED_H

#include "model/Color.h"

#include "controll/Motores.h"
#include "controll/MapManager.h"
#include "controll/Radar.h"

#include "BluetoothSerial.h"
#include "ArduinoJson.h"

#include <vector>
#include <random>
#include <string>
#include <map>

#pragma once

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

namespace N
{
    
    class SAeD
    {
        private:
        Motores motores;
        Color color;
        Radar radar;

        BluetoothSerial BTS;
        MapManager mapManager;
        
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

    public:
        SAeD();
        SAeD(int scaleX, int scaleY, float cellScale);

        void begin();

        void setNewMap(int *dimensions, int *initial);
        void principalLoop();
        void secondaryLoop();
        void received();
        void analise();

        std::vector<std::vector<bool>> getMap() { return mapManager.getMap(); }

        std::vector<char> setTarget(int *target);
        std::vector<int> getCurrentPosition();
        std::vector<int> getTargetPosition();
    };
}

#endif