#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H

#include <string>

//instantiates class for using it
class SystemState;



//type to hold opening hours
using OpeningHours = std::pair<int, int>; // Timer i 24-timers format



//funktions to check admin and user password inputs
bool checkAdminPassword(const std::string&);
bool checkUserPassword(const std::string&);

//check if current time is within opening hours
bool checkOpenTime(SystemState& state);

//set opening hours for a specific day
void setOpeningHours(SystemState& state, const std::string& day, int openHour, int closeHour);

//print current opening hours for all days
void printOpeningHours(const SystemState& state);

#endif // ACCESSCONTROL_H
