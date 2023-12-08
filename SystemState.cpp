//SystemState.cpp
#include "SystemState.h"



void SystemState::doorIsOpen() {
    isDoorOpenOrOpening = true;
}

void SystemState::doorIsClosed() {
    isDoorOpenOrOpening = false;
}

SystemState::SystemState() = default;


