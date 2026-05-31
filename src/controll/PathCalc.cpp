#include "PathCalc.h"

using namespace SAeD;
using namespace std;

std::vector<char> PathCalc::createPath(int* currentCell, int* targetCell)
{
    if (
        targetCell[0] == currentCell[0] &&
        targetCell[1] == currentCell[1])
    {
        // Alredy In Target - Já estou no alvo
        return {'A', 'I', 'T'};
    }

    int diference[2] = {currentCell[0] - targetCell[0], currentCell[1] - targetCell[1]};
    std::vector<char> path;

    for (int c = 0; c < (int)(sizeof(diference) / sizeof(diference[0])); c++)
    {
        int workNumber = diference[c];
        char workChar;

        if (c == 1)
        {
            if (workNumber < 1)
            {
                path.push_back('A');
            }
            else
            {
                path.push_back('D');
            }
        }

        if (workNumber < 1)
        {
            workNumber *= -1;
            workChar = 'S';
        }
        else
        {
            workChar = 'W';
        }

        do
        {
            path.push_back(workChar);
            workNumber--;
        } while (workNumber > 0);

        currentCell[c] += targetCell[c];
    }

    return path;
}