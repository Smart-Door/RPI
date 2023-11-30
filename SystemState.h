#ifndef SYSTEMSTATE_H
#define SYSTEMSTATE_H

#include <set>
#include <atomic>
#include <string>
#include <map>
#include <utility> // For std::pair
#include <iostream>

#include "AccessControl.h"


//defines type for holding the opening hours
using OpeningHours = std::pair<int, int>; // Timer i 24-timers format

class SystemState {
public:
    std::atomic<bool> isDoorOpenOrOpening{false}; //Flag if door is open or opening
    std::atomic<bool> programRunning{true}; //flag if program is running
    std::set<std::string> validDays; //days possible for opening hours
    std::map<std::string, OpeningHours> openingHours; //opening hours for the day

    //change isDoorOpenOrOpening state
    void doorIsOpen();
    void doorIsClosed();

    //3 wrong password attempts
    std::atomic<int> failedPasswordAttempts{0};
    std::atomic<bool> isSystemLocked{false};

    //userlogin og unlock system funktion
    //bool attemptUserLogin(const std::string& password);
    //void unlockSystem(const std::string& adminPassword);

    //constructor initializes default states and opening hours
    SystemState();
};

#endif // SYSTEMSTATE_H
