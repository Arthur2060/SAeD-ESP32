#ifndef DEMARCACAO_H
#define DEMARCACAO_H

#include <vector>
#include <stdint.h>

namespace SAeD
{
    struct area
    {
        int startCell[2];
        int endCell[2];
        std::vector<uint8_t> color;
        char *name;
    };

    class Demarcacao
    {
    private:
        std::vector<area> areas;

    public:
        int recieveingCell[2] = {};

        Demarcacao();
        Demarcacao(int rc1, int rc2);

        void setNewArea(std::vector<uint16_t> initialCell, std::vector<uint16_t> finalCell, std::vector<uint8_t> color, char *name);

        area getArea(char *name);
        area getCellArea(int *cellLocation);

        void validateAreas(std::vector<std::vector<bool>> map);
    };
}

#endif