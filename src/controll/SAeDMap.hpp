#include <cmath>
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
};