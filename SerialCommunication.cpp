#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <cstring>
#include <atomic>
#include <iostream>
#include "SerialCommunication.h"



void readCommand(int uart0_filestream) {
    constexpr int buffer_size = 256;
    unsigned char rx_buffer[buffer_size];
    std::string partialMessage;

    while (true) {
        memset(rx_buffer, '\0', buffer_size);
        int rx_length = read(uart0_filestream, (void*)rx_buffer, buffer_size - 1);
        if (rx_length > 0) {
            rx_buffer[rx_length] = '\0';

            // Append the received data to the partial message
            partialMessage += reinterpret_cast<char*>(rx_buffer);

            // Check if the partial message contains a newline character
            size_t newlinePos = partialMessage.find('\n');
            if (newlinePos != std::string::npos) {
                // Extract the complete message up to the newline
                std::string completeMessage = partialMessage.substr(0, newlinePos);

                // Process the complete message
                std::cout << "Received message: " << completeMessage << std::endl;

                // Check for substrings in the message content
                if (completeMessage.find("door_closed") != std::string::npos) {
                    std::cout << "Door closed" << std::endl;
                    isDoorOpenOrOpening = false;
                } else if (completeMessage.find("obstruction_detected") != std::string::npos) {
                    std::cout << "Hindring detecteret, doren genaabnes" << std::endl;
                    // Handle obstruction detection
                }

                // Update the partial message to remove the processed part
                partialMessage.erase(0, newlinePos + 1);
            }
        }
    }
}


int sendOpenCommand(int uart0_filestream) {

    unsigned char tx_buffer[20];
    memset(&tx_buffer, '\0', sizeof(tx_buffer));
    unsigned char *p_tx_buffer;
    p_tx_buffer = &tx_buffer[0];

    *p_tx_buffer++ = 'o';
    *p_tx_buffer++ = 'p';
    *p_tx_buffer++ = 'e';
    *p_tx_buffer++ = 'n';
    *p_tx_buffer++ = '_';
    *p_tx_buffer++ = 'd';
    *p_tx_buffer++ = 'o';
    *p_tx_buffer++ = 'o';
    *p_tx_buffer++ = 'r';
    *p_tx_buffer++ = '\n';

    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    if (count < 0) {
        printf("UART TX error\n");
        return 1;
    }
    return count;
}

