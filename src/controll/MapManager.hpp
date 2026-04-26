#include <vector>
#include "Demarcacao.hpp"

class MapManager {
    private:
        std::vector<std::vector<bool>> map;
        float distanciaDeQuadro = 0.3f;
        Demarcacao demarcacao;
    
    public:
        MapManager() {}

        MapManager(int scalex, int scaley, float distanciaDeQuadro) {
            for (int x = 0 ; x <= scalex ; x++) {
                for (int y = 0 ; y <= scaley ; y++) {
                    this->map[x].push_back(false);
                }
            }

            this->distanciaDeQuadro = distanciaDeQuadro;
            this->demarcacao = Demarcacao(map);
        }

        MapManager(std::vector<std::vector<bool>> map, float distanciaDeQuadro) {
            this->map = map;
            this->distanciaDeQuadro = distanciaDeQuadro;
            this->demarcacao = Demarcacao(map);
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

        Demarcacao getDemarcacao() {
            return this->demarcacao;
        }

        std::vector<std::vector<bool>> getMap() {
            return this->map;
        }

        void setNewMap(int scalex, int scaley) {
            for (int x = 0 ; x <= scalex ; x++) {
                for (int y = 0 ; y <= scaley ; y++) {
                    this->map[x].push_back(false);
                }
            }

            this->demarcacao;
        }

        void setNewMap(std::vector<std::vector<bool>> map) {
            this->map = map;
            this->demarcacao.setMap(this->map);
        }

        void setNewMap(int scalex, int scaley) {
            for (int x = 0 ; x <= scalex ; x++) {
                for (int y = 0 ; y <= scaley ; y++) {
                    this->map[x].push_back(false);
                }
            }
            
            this->demarcacao.setMap(this->map);
        }

        void setNewMap(int* scale) {
            for (int x = 0 ; x <= scale[0] ; x++) {
                for (int y = 0 ; y <= scale[1] ; y++) {
                    this->map[x].push_back(false);
                }
            }

            this->demarcacao.setMap(this->map);
        }
    };