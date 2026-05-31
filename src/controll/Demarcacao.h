#ifndef DEMARCACAO_H
#define DEMARCACAO_H

#include <vector>
#include <stdint.h>

#include "model/ColorDetect.h"

namespace SAeD
{
    struct area
    {
        int startCell[2];
        int endCell[2];
        color areaColor;
        char *name;
    };

    class Demarcacao
    {
    private:
    public:
        int recieveingCell[2] = {};
        std::vector<area> areas;

        Demarcacao();
        Demarcacao(int rc1, int rc2);

        void setNewArea(std::vector<uint16_t> initialCell, std::vector<uint16_t> finalCell, color color, char *name);

        area getArea(char *name);
        area getCellArea(int *cellLocation);

        void validateAreas(std::vector<std::vector<bool>> map);
    };
}

#endif