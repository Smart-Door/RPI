#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H

#include <string>
#include <map>
#include <utility> // For std::pair


//adgangskoder
const std::string ADMIN_PASSWORD = "1122"; // Erstat med et sikkert password
const std::string USER_PASSWORD = "2211";

// Definer en type til at holde åbningstider (start, slut)
using OpeningHours = std::pair<int, int>; // Timer i 24-timers format

//funktioner til at tjekke admin og user passwords input
bool checkAdminPassword(const std::string& password);
bool checkUserPassword(const std::string& password);

//bekræfter det er inden for åbningstid
bool checkOpenTime();
//ændre åbningstid
void setOpeningHours(const std::string& day, int openHour, int closeHour);
//print de nuværende konfigurerede åbnignstider.
void printOpeningHours();

#endif // ACCESSCONTROL_H
