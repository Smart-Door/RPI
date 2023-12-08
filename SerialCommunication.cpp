//SerialCommunication.cpp
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include "SerialCommunication.h"

#include "SystemState.h"

SerialCommunication::SerialCommunication() = default;

bool SerialCommunication::setupUART() {
    //setupUART attempts
    constexpr int MAX_SETUP_ATTEMPTS = 5;
    constexpr int SETUP_RETRY_DELAY_MS = 500;

    for (int attempt = 0; attempt < MAX_SETUP_ATTEMPTS; ++attempt) {
        this->uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);

        if (this->uart0_filestream != -1) {
            //if open is successful, configure UART
            struct termios options;
            tcgetattr(this->uart0_filestream, &options);
            options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
            options.c_iflag = IGNPAR;
            options.c_oflag = 0;
            options.c_lflag = 0;
            tcflush(this->uart0_filestream, TCIFLUSH);
            if (tcsetattr(this->uart0_filestream, TCSANOW, &options) != -1) {
                return true;
            }
            close(this->uart0_filestream); //close if settings fail
        }

        std::cerr << "Attempt " << attempt + 1 << " to open UART failed." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(SETUP_RETRY_DELAY_MS));
    }

    return false;
}

//read commands from UART and handle them
void SerialCommunication::readCommand(SystemState& state) {
    constexpr int bufferSize = 256;
    unsigned char rxBuffer[bufferSize];
    std::string partialMessage;
    int readErrorCount = 0;
    constexpr int MAX_READ_ERRORS = 3;

    while (true) {
        memset(rxBuffer, 0, bufferSize);
        int rxLength = read(this->uart0_filestream, static_cast<void*>(rxBuffer), bufferSize - 1);

        //if something recieved
        if (rxLength > 0) {
            readErrorCount = 0;
            rxBuffer[rxLength] = '\0';
            // **DEBUGGING ** std::cout << "Received: " << rxBuffer << std::endl; //debugging line
            partialMessage.append(reinterpret_cast<char*>(rxBuffer));

            // Handling complete messages
            size_t newlinePos = partialMessage.find('\n');
            if (newlinePos != std::string::npos) {
                std::string completeMessage = partialMessage.substr(0, newlinePos);
                //**DEBUGGING** std::cout << "Handling message: " << completeMessage << std::endl; //debugging line
                SerialCommunication::handleReceivedMessage(completeMessage, state); //processing the message
                partialMessage.erase(0, newlinePos + 1);
            }
        }

        //if error with recived
        else if (rxLength < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No data available right now, wait a bit before retrying
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            } else {
                // Handle actual read errors
                perror("Read error");
                readErrorCount++;
                if (readErrorCount >= MAX_READ_ERRORS) {
                    std::cerr << "Too many read errors, attempting to reset UART." << std::endl;
                    if (!setupUART()) {
                        std::cerr << "Failed to reset UART. Exiting read loop." << std::endl;
                        break;
                    }
                readErrorCount = 0;
                }
            }
        }

        //no data received, but no error (e.g., non-blocking read)
        else {
            readErrorCount = 0; //reset error count as there's no error
        }
    }
}


//send command to open the door through UART
int SerialCommunication::sendOpenCommand() const {
    const char* command = "open_door\n";
    size_t length = strlen(command);
    ssize_t count = write(this->uart0_filestream, command, length);
    if (count < 0) {
        perror("UART TX error");
        return -1; //returning negative value to indicate error
    }
    return static_cast<int>(count);
}


//handle messages received via UART
void SerialCommunication::handleReceivedMessage(const std::string& message, SystemState& state) {
// **DEBUGGING ** std::cout << "Received in handleReceivedMessage: " << message << std::endl;
    if (message == "door_closed") {
        state.doorIsClosed();
        std::cout << "\n*** Døren er lukket ***" << std::endl;
    }
    else if (message == "obstruction_detected") {
        std::cout << "\n*** Døren kan ikke lukke *** \nPrøver igen om 10 sekunder" << std::endl;
    }
}
