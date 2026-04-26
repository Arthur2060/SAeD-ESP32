#include <vector>
#include <cmath>

class PathCalc {
    private:
        int currentCell[2] = {0, 0};
        int targetCell[2] = {0, 0};
        std::vector<std::vector<bool>> map;
    public:
        PathCalc() {}

        PathCalc(std::vector<std::vector<bool>> map) {
            this->map = map;
        }

        std::vector<char> moveToTarget() {
            if (
                targetCell[0] == currentCell[0] &&
                targetCell[1] == currentCell[1]    
            ) {
                // Alredy in Target - Já estou no alvo
                return {'A', 'I', 'T'};
            }

            int diference[2] = {currentCell[0] - targetCell[0], currentCell[1] - targetCell[1]};
            std::vector<char> path;

            for (int c = 0 ; c <= sizeof(diference) ; c++) {
                int workNumber = diference[c];
                char workChar;
                
                if (workNumber < 1) {
                    workNumber *= -1;
                    (c == 0) ? workChar = 'S' : workChar = 'A';
                } else {
                    (c == 0) ? workChar = 'W' : workChar = 'D';
                }

                do {
                    path.push_back(workChar);
                    workNumber--;
                } while(workNumber > 0);

                currentCell[c] += workNumber;
            }

            return path;
        }

        std::vector<char> setTarget(int target[2]) {
            if 
            (
                !(target[0] >= 0 && target[0] <= sizeof(map) &&
                target[1] >= 0 && target[1] <= sizeof(map[target[0]]))
            ) {
                // Invalid target - Alvo inválido
                return {'I', 'T'};
            }

            this->targetCell[0] = target[0];
            this->targetCell[1] = target[1];

            return moveToTarget();
        }

        void setMap(std::vector<std::vector<bool>> map) {
            this->map = map;
        }
};