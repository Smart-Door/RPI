#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <atomic>

#include "SystemState.h"

extern std::atomic<bool> isDoorOpenOrOpening;

bool setupUART(int& uart0_filestream);
void readCommand(int& uart0_filestream, SystemState& state);
int sendOpenCommand(int uart0_filestream);
void handleReceivedMessage(const std::string& message, SystemState& state);

#endif // SERIALCOMMUNICATION_H
