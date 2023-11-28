#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <atomic>

extern std::atomic<bool> isDoorOpenOrOpening;

void readCommand(int uart0_filestream);


int sendOpenCommand(int uart0_filestream);


#endif // SERIALCOMMUNICATION_H
