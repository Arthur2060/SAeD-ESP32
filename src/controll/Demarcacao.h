#ifndef DEMARCACAO_H
#define DEMARCACAO_H

#include <vector>
#include <stdint.h>

namespace N
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
        std::vector<std::vector<bool>> map;

    public:
        std::vector<area> areas;
        int recieveingCell[2] = {};

        Demarcacao() {}
        Demarcacao(std::vector<std::vector<bool>> map, int rc1, int rc2);
        Demarcacao(int scaleX, int scaleY);

        bool setNewArea(std::vector<uint16_t> initialCell, std::vector<uint16_t> finalCell, std::vector<uint8_t> color, char *name);
        area getArea(char *name);
        area getCellArea(int *cellLocation);
        void setMap(std::vector<std::vector<bool>> map);
        void validateAreas();
    };
}

#endif