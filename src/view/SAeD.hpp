#include "model/Motores.hpp"
#include "model/Color.hpp"

#include "controll/MapManager.hpp"
#include "controll/SAeDStateMachine.hpp"
#include "controll/Radar.hpp"

class SAeD {
    private:
        Motores motores;
        Color color;
        Radar radar;

        MapManager mapManager;

        SAeDStateDispatch dispatchState;
        SAeDStateMap mapState;
        SAeDStateNewItem newItemState;

    public:
        SAeD() {this->mapManager = MapManager(10, 10, 0.3);}

        void begin() {
            motores.begin();
            radar.begin();
            color.begin();
        }
};