#include "Demarcacao.h"

using namespace N;
using namespace std;

Demarcacao::Demarcacao(std::vector<std::vector<bool>> map, int rc1, int rc2)
{
    this->map = map;
    this->recieveingCell[0] = rc1;
    this->recieveingCell[1] = rc2;
}

Demarcacao::Demarcacao(int scaleX, int scaleY)
{
    std::vector<std::vector<bool>> map;

    for (int x = 0; x <= scaleX; x++)
    {
        map.push_back({});
        std::vector<bool> targetLine = map[x];
        for (int y = 0; y <= scaleY; y++)
        {
            map[x].push_back(false);
        }
    }

    this->map = map;
}

bool Demarcacao::setNewArea(std::vector<uint16_t> initialCell, std::vector<uint16_t> finalCell, std::vector<uint8_t> color, char *name)
{
    if (
        initialCell[0] < 0 ||
        initialCell[1] < 0 ||
        initialCell[0] > sizeof(map) ||
        initialCell[1] < sizeof(map[0]) ||

        finalCell[0] < 0 ||
        finalCell[1] < 0 ||
        finalCell[0] > sizeof(map) ||
        finalCell[1] < sizeof(map[0]))
    {
        return false;
    }

    area newArea;

    newArea.startCell[0] = initialCell[0];
    newArea.startCell[1] = initialCell[1];

    newArea.name = name;

    newArea.endCell[0] = finalCell[0];
    newArea.endCell[1] = finalCell[1];

    newArea.color = color;

    areas.push_back(newArea);
    return true;
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

void Demarcacao::setMap(std::vector<std::vector<bool>> map)
{
    this->map = map;
    validateAreas();
}

void Demarcacao::validateAreas()
{
    for (int c = 0; c <= sizeof(areas); c++)
    {
        if (
            areas[c].startCell[0] < 0 ||
            areas[c].startCell[1] < 0 ||
            areas[c].startCell[0] > sizeof(map) ||
            areas[c].startCell[1] < sizeof(map[areas[c].startCell[0]]) ||

            areas[c].endCell[0] < 0 ||
            areas[c].endCell[1] < 0 ||
            areas[c].endCell[0] > sizeof(map) ||
            areas[c].endCell[1] < sizeof(map[areas[c].endCell[0]]))
        {
            areas.erase(areas.begin() + c);
        }
    }
}