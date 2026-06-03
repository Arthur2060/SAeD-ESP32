#include "PathCalc.h"

using namespace SAeD;
using namespace std;

std::vector<char> PathCalc::createPath(int *currentCell, int *targetCell)
{
    int diference[2] = {currentCell[0] - targetCell[0], currentCell[1] - targetCell[1]};
    std::vector<char> path;

    if (diference[0] > 0)
    {
        for (int c2 = diference[0]; c2 > 0; c2--)
        {
            path.push_back('W');
        }
        path.push_back('D');
    }
    else if (diference[0] < 0)
    {
        for (int c2 = diference[0]; c2 < 0; c2++)
        {
            path.push_back('S');
        }
        path.push_back('A');
    }

    if (diference[1] > 0)
    {
        for (int c2 = diference[1]; c2 > 0; c2--)
        {
            path.push_back('W');
        }
    }
    else if (diference[1] < 0)
    {
        for (int c2 = diference[1]; c2 < 0; c2++)
        {
            path.push_back('S');
        }
    }

    return path;
}