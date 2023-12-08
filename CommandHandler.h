//CommandHandler.h
#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "SystemState.h"
#include "SerialCommunication.h"
#include "TimeUtility.h"
#include "PasswordManager.h"

#include <iostream>
#include <limits>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

class CommandHandler {
public:
    CommandHandler(SystemState& state, SerialCommunication& serialComm, TimeUtility& timeUtility);

    void runCommandLoop(const std::map<std::string, std::string>& credentials);
    void handleOpenCommand(const std::map<std::string, std::string>& credentials);
    void handleSkiftCommand(const std::map<std::string, std::string>& credentials);
private:
    SystemState& state;
    SerialCommunication& serialComm;
    TimeUtility& timeUtility;

};

#endif // COMMANDHANDLER_H
