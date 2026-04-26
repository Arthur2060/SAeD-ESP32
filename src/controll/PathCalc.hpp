#include <vector>
#include <cmath>

class PathCalc {
    private:
        int currentCell[2] = {0, 0};
        int targetCell[2] = {0, 0};
    public:
        PathCalc() {}

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
            }

            return path;
        }
};