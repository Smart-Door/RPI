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


// bool SystemState::attemptUserLogin(const std::string& password) {

//     //if system is locked message
//     if (isSystemLocked.load()) {
//         std::cout << "System is locked. Please enter admin password to unlock." << std::endl;
//         return false;
//     }

//     //checks userpassword and increments failed password counter.
//     //Locks with 3 failed
//     if (!checkUserPassword(password)) {
//         failedPasswordAttempts++;
//         if (failedPasswordAttempts.load() >= 3) {
//             isSystemLocked = true;
//             std::cout << "System locked due to too many failed attempts." << std::endl;
//         }
//         return false;
//     }

//     failedPasswordAttempts = 0; //resets the failed password counter
//     return true; //successful login
// }

// //unlock the system when in password attempt locked state
// void SystemState::unlockSystem(const std::string& adminPassword) {
//     if (checkAdminPassword(adminPassword)) {
//         isSystemLocked = false;
//         failedPasswordAttempts = 0;
//         std::cout << "System unlocked." << std::endl;
//     } else {
//         std::cout << "Incorrect admin password." << std::endl;
//     }
// }
