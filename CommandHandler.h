// CommandHandler.h
#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "SystemState.h"
#include "SerialCommunication.h"
#include "AccessControl.h"
#include "GPIOControl.h"


#include <iostream>
#include <limits>

//main loop to process user commands
void runCommandLoop(int uart0_filestream, SystemState& state);
//handle 'open' command to open the door
void handleOpenCommand(int uart0_filestream, SystemState& state);
//handle 'skift' command to change opening hours
void handleSkiftCommand(SystemState& state);
//verify if the provided password is correct
bool verifyPassword(SystemState& state, const std::string& password, bool isAdmin);

int getValidHour(const std::string& prompt);

#endif // COMMANDHANDLER_H
