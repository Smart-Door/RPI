//PasswordManager.h
#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H
#include "SystemState.h"

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <atomic>

class PasswordManager {
public:
    static bool authenticateOpenCommand(const std::map<std::string, std::string>& credentials, SystemState& state);
    static bool authenticateSkiftCommand(const std::map<std::string, std::string>& credentials, SystemState& state);
    static bool unlockSystem(const std::map<std::string, std::string>& credentials, SystemState& state);
    static std::map<std::string, std::string> readUserCredentials(const std::string& filename);

private:
    static std::pair<std::string, std::string> promptForCredentials();
    static void handleFailedAttempt(SystemState& state);
    static bool verifyPassword(const std::map<std::string, std::string>& credentials, const std::string& username, const std::string& password);

};

#endif //PASSWORDMANAGER_H
