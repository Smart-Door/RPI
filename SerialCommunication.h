//SerialCommunication.h
#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <atomic>

#include "SystemState.h"



class SerialCommunication {
public:
    SerialCommunication();
    bool setupUART();
    void readCommand(SystemState& state);
    int sendOpenCommand() const;
    static void handleReceivedMessage(const std::string& message, SystemState& state);

private:
    int uart0_filestream = -1;
};


#endif // SERIALCOMMUNICATION_H
