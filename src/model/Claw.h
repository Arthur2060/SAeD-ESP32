#ifndef CLAW_H
#define CLAW_H

#include <Arduino.h>
#include <map>

#define TX_PIN 1
#define RX_PIN 3

namespace SAeD
{

    enum class ClawState
    {
        IDLE,
        GET,
        PUT
    };

    class Claw
    {
    private:
        HardwareSerial serial = Serial1;
        ClawState currentState = ClawState::IDLE;

        std::map<ClawState, int> clawCurrentState = {
            {ClawState::IDLE, 0},
            {ClawState::GET, 1},
            {ClawState::PUT, 2}};

        std::map<ClawState, ClawState> clawStateMachine = {
            {ClawState::IDLE, ClawState::GET},
            {ClawState::GET, ClawState::PUT},
            {ClawState::PUT, ClawState::IDLE}};

    public:
        Claw();
        void begin();

        void nextState();
    };
}

#endif