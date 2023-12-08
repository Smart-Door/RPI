//TimeUtility.cpp
#include "TimeUtility.h"
#include <chrono>
#include <iostream>

TimeUtility::TimeUtility() {

    validDays = {"Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag", "Søndag"};

    // Hardcoded default opening hours
    openingHours.push_back({"Mandag", {9, 17}});
    openingHours.push_back({"Tirsdag", {9, 17}});
    openingHours.push_back({"Onsdag", {9, 17}});
    openingHours.push_back({"Torsdag", {9, 17}});
    openingHours.push_back({"Fredag", {9, 17}});
    openingHours.push_back({"Lørdag", {9, 17}});
    openingHours.push_back({"Søndag", {9, 17}});
}

bool TimeUtility::isValidDay(const std::string& day) const {
    return validDays.find(day) != validDays.end();
}

//check if current time is within opening hours
bool TimeUtility::checkOpenTime() const {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    //std::cout << "Current time: " << std::ctime(&currentTime) << "\n";

    std::tm *localTime = std::localtime(&currentTime);

    const std::string daysOfWeek[7] = {"Søndag", "Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag"};
    std::string currentDay = daysOfWeek[localTime->tm_wday];

    for (const auto& [day, hours] : openingHours) {
        if (day == currentDay) {
            return localTime->tm_hour >= hours.first && localTime->tm_hour < hours.second;
        }
    }
    return false;
}

//set opening hours for a specific day
void TimeUtility::setOpeningHours(const std::string &day, int openHour, int closeHour) {
    for (auto& [d, hours] : openingHours) {
        if (d == day) {
            hours = {openHour, closeHour};
            return; // Update found day and return
        }
    }
}

//print current opening hours for all days
void TimeUtility::printOpeningHours() {
    for (const auto& [day, hours] : openingHours) {
        std::cout << day << ": " << hours.first << ":00 - " << hours.second << ":00" << std::endl;
    }
}

//check valid hour
int TimeUtility::getValidHour(const std::string& prompt) {
    int hour;
    std::cout << prompt;
    while (!(std::cin >> hour) || hour < 0 || hour > 23) {
        std::cout << "\nUgyldig time, prøv igen: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore();
    return hour;
}

