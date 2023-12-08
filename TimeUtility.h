//TimeUtility.h
#ifndef TIMEUTILITY_H
#define TIMEUTILITY_H

#include <string>
#include <vector>
#include <set>

// Defines type for holding the opening hours
using OpeningHours = std::pair<int, int>; // Hours in 24-hour format

class TimeUtility {
public:
    TimeUtility();
    bool isValidDay(const std::string& day) const;
    bool checkOpenTime() const;
    void setOpeningHours(const std::string&, int, int);
    void printOpeningHours();
    static int getValidHour(const std::string& prompt);

private:
    std::vector<std::pair<std::string, OpeningHours>> openingHours;
    std::set<std::string> validDays;
};

#endif // TIMEUTILITY_H

