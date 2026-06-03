#include "Claw.h"

using namespace SAeD;
using namespace std;

Claw::Claw() {
}

void Claw::begin() {
    this->serial.begin(9600, SERIAL_8N1, TX_PIN, RX_PIN);
    serial.println(clawCurrentState[currentState]);
}

void Claw::nextState() {
    currentState = clawStateMachine[currentState];
    serial.println(clawCurrentState[currentState]);
}