#include "AccessControl.h"
#include "SystemState.h"
#include <chrono>
#include <iostream>


//hardcoded password for admin and user
const std::string ADMIN_PASSWORD = "1122";
const std::string USER_PASSWORD = "2211";

//check if current time is within opening hours
bool checkOpenTime(SystemState& state) {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm *localTime = std::localtime(&currentTime);

    const std::string daysOfWeek[7] = {"Søndag", "Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag"};
    std::string currentDay = daysOfWeek[localTime->tm_wday];

    auto it = state.openingHours.find(currentDay);
    if (it != state.openingHours.end()) {
        int openHour = it->second.first;
        int closeHour = it->second.second;
        return localTime->tm_hour >= openHour && localTime->tm_hour < closeHour;
    }
    return false; // Lukket, hvis der ikke er defineret åbningstider for denne dag
}

//set opening hours for a specific day
void setOpeningHours(SystemState& state, const std::string& day, int openHour, int closeHour) {
    state.openingHours[day] = std::make_pair(openHour, closeHour);
}

//print current opening hours for all days
void printOpeningHours(const SystemState& state) {
    for (const auto& day : state.openingHours) {
        std::cout << day.first << ": " << day.second.first << ":00 - "
                  << day.second.second << ":00" << std::endl;
    }
}


bool checkAdminPassword(const std::string& password) {
    return password == ADMIN_PASSWORD;
}

bool checkUserPassword(const std::string& password) {
    return password == USER_PASSWORD;
}

