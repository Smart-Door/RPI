//PasswordManager.cpp
#include "PasswordManager.h"
#include <chrono>
#include <thread>


bool PasswordManager::authenticateOpenCommand(const std::map<std::string, std::string>& credentials, SystemState& state) {
    auto [username, password] = promptForCredentials();
    if (verifyPassword(credentials, username, password)) {
        state.failedPasswordAttempts = 0;
        return true;
    } else {
        handleFailedAttempt(state);
        return false;
    }
}

bool PasswordManager::authenticateSkiftCommand(const std::map<std::string, std::string>& credentials, SystemState& state) {
    auto [username, password] = promptForCredentials();
    if (username == "admin" && verifyPassword(credentials, username, password)) {
        state.failedPasswordAttempts = 0;
        return true;
    } else {
        handleFailedAttempt(state);
        return false;
    }
}

std::pair<std::string, std::string> PasswordManager::promptForCredentials() {
    std::string username, password;

    std::cout << "Indtast brugernavn: ";
    std::getline(std::cin, username);

    std::cout << "Indtast password for " << username << ": ";
    std::getline(std::cin, password);
    return std::make_pair(username, password);
}


std::map<std::string, std::string> PasswordManager::readUserCredentials(const std::string& filename) {
    std::map<std::string, std::string> credentials;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string username, password;
        if (std::getline(iss, username, ',') && std::getline(iss, password)) {
            credentials[username] = password;
        }
    }
    return credentials;
}

bool PasswordManager::verifyPassword(const std::map<std::string, std::string>& credentials,
                    const std::string& username,
                    const std::string& password) {
    auto it = credentials.find(username);
    if (it != credentials.end()) {
        return it->second == password;
    }
    return false;
}











bool PasswordManager::unlockSystem(const std::map<std::string, std::string>& credentials, SystemState& state) {
    if (state.isSystemLocked) {
        std::cout << "Vent venligst 3 sekunder før næste forsøg." << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    auto [username, password] = promptForCredentials();
    if (verifyPassword(credentials, username, password)) {
        state.failedPasswordAttempts = 0;
        state.isSystemLocked = false;
        return true;
    } else {
        state.failedPasswordAttempts++;
        return false;
    }
}

void PasswordManager::handleFailedAttempt(SystemState& state) {
    state.failedPasswordAttempts++;
    if (state.failedPasswordAttempts >= 3) {
        state.isSystemLocked = true;
        std::cout << "System låst efter 3 fejlede forsøg." << std::endl;

    }
}
