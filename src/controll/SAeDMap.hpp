#include <vector>

class SAeDMap {
    private:
        std::vector<std::vector<bool>> map;
        float distanciaDeQuadro = 0.3f;

    public:
        SAeDMap(int scalex, int scaley, float distanciaDeQuadro) {
            for (int x = 0 ; x <= scalex ; x++) {
                for (int y = 0 ; y <= scaley ; y++) {
                    this->map[x].push_back(false);
                }
            }

            this->distanciaDeQuadro = distanciaDeQuadro;
        }

        float getDistanciaDeQuadro() {
            return distanciaDeQuadro;
        } 

        float setDistanciaDeQuadro(float distancia) {
            if (distancia > 0) {
                distanciaDeQuadro = distancia;
            }
        }

        bool addObstacle(float distanceX, float distanceY) {
            float x = distanceX / distanciaDeQuadro;
            float y = distanceY / distanciaDeQuadro;

            if (!map[x][y]) {
                map[x][y] = !map[x][y];
                return true;
            }

            return false;
        }
};