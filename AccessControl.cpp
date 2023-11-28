#include "AccessControl.h"
#include <chrono>
#include <iostream>

std::map<std::string, OpeningHours> openingHours;

bool checkOpenTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm *localTime = std::localtime(&currentTime);

    // Find dagens navn (Mandag, Tirsdag, osv.)
    std::string daysOfWeek[7] = {"Søndag", "Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag"};
    std::string currentDay = daysOfWeek[localTime->tm_wday];

    // Tjek om der er åbningstider defineret for denne dag
    auto it = openingHours.find(currentDay);
    if (it != openingHours.end()) {
        int openHour = it->second.first;
        int closeHour = it->second.second;
        return localTime->tm_hour >= openHour && localTime->tm_hour < closeHour;
    }

    return false; // Lukket, hvis der ikke er defineret åbningstider for denne dag
}

void setOpeningHours(const std::string& day, int openHour, int closeHour) {
    openingHours[day] = std::make_pair(openHour, closeHour);
}

void printOpeningHours() {
    for (const auto& day : openingHours) {
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