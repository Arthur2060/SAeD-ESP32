#include <vector>
#include <cmath>

class PathCalc
{
private:
    int currentCell[2] = {0, 0};
    int targetCell[2] = {0, 0};
    std::vector<std::vector<bool>> map;

public:
    PathCalc() {}

    PathCalc(std::vector<std::vector<bool>> map)
    {
        this->map = map;
    }

    PathCalc(int scaleX, int scaleY)
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

    std::vector<char> moveToTarget();
    std::vector<char> setTarget(int *target);
    std::vector<char> setTarget(int x, int y);
    void setCurrentCell(int x, int y) {
        currentCell[0] = x;
        currentCell[1] = y;
    }

    std::vector<int> getCurrentCell() {
        return {currentCell[0], currentCell[1]};
    }

    void setMap(std::vector<std::vector<bool>> map);
};

void PathCalc::setMap(std::vector<std::vector<bool>> map)
{
    this->map = map;
    this->currentCell[0] = 0;
    this->currentCell[1] = 0;
}

std::vector<char> PathCalc::setTarget(int x, int y)
{
    if (
        !(x >= 0 && x <= sizeof(map) && y >= 0 && y <= sizeof(map[x])) ||
        (map[x][y])
    )
    {
        // Invalid target - Alvo inválido
        return {'I', 'T'};
    }

    this->targetCell[0] = x;
    this->targetCell[1] = y;

    return moveToTarget();
}

std::vector<char> PathCalc::setTarget(int *target)
{
    if (
        !(target[0] >= 0 && target[0] <= sizeof(map) &&
          target[1] >= 0 && target[1] <= sizeof(map[target[0]])))
    {
        // Invalid Target - Alvo inválido
        return {'I', 'T'};
    }

    this->targetCell[0] = target[0];
    this->targetCell[1] = target[1];

    return moveToTarget();
}

std::vector<char> PathCalc::moveToTarget()
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

    for (int c = 0; c <= sizeof(diference); c++)
    {
        int workNumber = diference[c];
        char workChar;

        if (workNumber < 1)
        {
            workNumber *= -1;
            (c == 0) ? workChar = 'S' : workChar = 'A';
        }
        else
        {
            (c == 0) ? workChar = 'W' : workChar = 'D';
        }

        do
        {
            path.push_back(workChar);
            workNumber--;
        } while (workNumber > 0);

        currentCell[c] += workNumber;
    }

    return path;
}