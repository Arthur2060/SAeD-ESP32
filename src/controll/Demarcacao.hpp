#include <vector>

struct area
{
    int startCell[2];
    int endCell[2];
    char* name;
};

class Demarcacao {
    private:
        std::vector<std::vector<bool>> map;
        std::vector<area> areas;

    public:
        Demarcacao(std::vector<std::vector<bool>> map) {
            this->map = map;
        }

        bool setNewArea(int* initialCell, int* finalCell, char* name) {
            if (
                initialCell[0] < 0 ||
                initialCell[1] < 0 ||
                initialCell[0] > sizeof(areas) ||
                initialCell[1] < sizeof(areas[0]) ||

                finalCell[0] < 0 ||
                finalCell[1] < 0 ||
                finalCell[0] > sizeof(areas) ||
                finalCell[1] < sizeof(areas[0])
            ) {
                return false;
            }

            area newArea;

            newArea.startCell[0] = initialCell[0];
            newArea.startCell[1] = initialCell[1];

            newArea.name = name;

            newArea.endCell[0] = finalCell[0];
            newArea.endCell[1] = finalCell[1];
            
            areas.push_back(newArea);
            return true;
        }

        area getSomeArea(char* name) {
            for (area area : areas)
            {
                if (area.name == name) {
                    return area;
                }
            }
        }

        area getCellArea(int* cellLocation) {
            for (area area : areas) {
                if (
                    cellLocation[0] > area.startCell[0] &&
                    cellLocation[0] < area.endCell[0] &&
                    cellLocation[1] > area.startCell[1] &&
                    cellLocation[1] < area.endCell[1]
                ) {
                    return area;
                }
            }
        }
};