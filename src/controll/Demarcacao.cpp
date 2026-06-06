#include "Demarcacao.h"

using namespace SAeD;
using namespace std;

Demarcacao::Demarcacao()
{
    this->recieveingCell[0] = 1;
    this->recieveingCell[1] = 1;
    
    this->startCell[0] = 0;
    this->startCell[1] = 0;
}

Demarcacao::Demarcacao(int rc1, int rc2)
{
    this->recieveingCell[0] = rc1;
    this->recieveingCell[1] = rc2;

    color any;

    any.MaxB = 99999;
    any.MaxR = 99999;
    any.MaxG = 99999;
    any.minB = 0;
    any.minR = 0;
    any.minG = 0;
    
    this->setNewArea({2, 2}, {2, 2}, any, "AnyColor");
}

void Demarcacao::setNewArea(std::vector<uint16_t> initialCell, std::vector<uint16_t> finalCell, color color, const char* name)
{
    area newArea;

    newArea.startCell[0] = initialCell[0];
    newArea.startCell[1] = initialCell[1];

    newArea.name = name;

    newArea.endCell[0] = finalCell[0];
    newArea.endCell[1] = finalCell[1];

    newArea.areaColor = color;

    areas.push_back(newArea);
}

area Demarcacao::getArea(char *name)
{
    area target;

    for (area area : areas)
    {
        if (area.name == name)
        {
            target = area;
        }
    }

    return target;
}

area Demarcacao::getCellArea(int *cellLocation)
{
    area target;

    for (area area : areas)
    {
        if (
            cellLocation[0] > area.startCell[0] &&
            cellLocation[0] < area.endCell[0] &&
            cellLocation[1] > area.startCell[1] &&
            cellLocation[1] < area.endCell[1])
        {
            target = area;
        }
    }

    return target;
}

void Demarcacao::validateAreas(std::vector<std::vector<bool>> map)
{
    for (int c = 0; c <= sizeof(areas); c++)
    {
        area area = areas[c];

        if (
            area.startCell[0] < 0 ||
            area.startCell[1] < 0 ||
            area.startCell[0] > sizeof(map) ||
            area.startCell[1] < sizeof(map[0]) ||

            area.endCell[0] < 0 ||
            area.endCell[1] < 0 ||
            area.endCell[0] > sizeof(map) ||
            area.endCell[1] < sizeof(map[0]))
        {
            areas.erase(areas.begin() + c);
        }
    }
}