#include "SystemState.h"

SystemState::SystemState() : validDays({"Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag", "Søndag"}) {
    
    // Hardcoded default opening hours
    openingHours["Mandag"] = std::make_pair(9, 17); // 9:00 - 17:00
    openingHours["Tirsdag"] = std::make_pair(9, 17);
    openingHours["Onsdag"] = std::make_pair(9, 17);
    openingHours["Torsdag"] = std::make_pair(9, 17);
    openingHours["Fredag"] = std::make_pair(9, 17);
    openingHours["Lørdag"] = std::make_pair(9, 17);
    openingHours["Søndag"] = std::make_pair(9, 17);
}

void SystemState::doorIsOpen() {
    isDoorOpenOrOpening = true;
}

void SystemState::doorIsClosed() {
    isDoorOpenOrOpening = false;
}


