#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <string>
//#include <cstring>
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include "SerialCommunication.h"

#include "SystemState.h"

// //constants for retry uart mechanism
// constexpr int MAX_RETRY_COUNT = 5;
// constexpr int RETRY_DELAY_MS = 1000;


//

// //read commands from UART and handle them
// void readCommand(int uart0_filestream, SystemState& state) {
//     constexpr int bufferSize = 256;
//     unsigned char rxBuffer[bufferSize];
//     std::string partialMessage;
//     int retryCount = 0;

//     while (true) {
//         memset(rxBuffer, '\0', bufferSize);
//         int rxLength = read(uart0_filestream, (void*)rxBuffer, bufferSize - 1);


//         //handling read errors
//         if (rxLength < 0) {
//             perror("Read error");
//             retryCount++;
//             // Attempt to reopen UART after max retries
//             if (retryCount >= MAX_RETRY_COUNT) {
//                 std::cerr << "Max retry attempts reached. Attempting to reopen UART." << std::endl;
//                 if (!setupUART(uart0_filestream)) {
//                     std::cerr << "Failed to reopen UART. Exiting read loop." << std::endl;
//                     break;
//                 }
//                 retryCount = 0; // Reset the retry count after successful reopen
//             }
//             std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_DELAY_MS));
//             continue;
//         }
//         retryCount = 0; //reset retry count on successful read

//         //process received data
//         if (rxLength > 0) {
//             rxBuffer[rxLength] = '\0';
//             std::cout << "Received: " << rxBuffer << std::endl; //debugging line
//             partialMessage.append(reinterpret_cast<char*>(rxBuffer));

//             // Handling complete messages
//             size_t newlinePos = partialMessage.find('\n');
//             if (newlinePos != std::string::npos) {
//                 std::string completeMessage = partialMessage.substr(0, newlinePos);
//                 std::cout << "Handling message: " << completeMessage << std::endl; //debugging line
//                 handleReceivedMessage(completeMessage, state); //processing the message
//                 partialMessage.erase(0, newlinePos + 1);
//             }
//         }

//         //prevents buffer from growing crazy
//         if (partialMessage.length() > 1000) { 
//             partialMessage.clear();
//         }
//     }
// }

//read commands from UART and handle them
void readCommand(int& uart0_filestream, SystemState& state) {
    constexpr int bufferSize = 256;
    unsigned char rxBuffer[bufferSize];
    std::string partialMessage;
    int readErrorCount = 0;
    constexpr int MAX_READ_ERRORS = 3;

    while (true) {
        memset(rxBuffer, 0, bufferSize);
        int rxLength = read(uart0_filestream, static_cast<void*>(rxBuffer), bufferSize - 1);

        //if something recieved
        if (rxLength > 0) {
            readErrorCount = 0;
            rxBuffer[rxLength] = '\0';
            std::cout << "Received: " << rxBuffer << std::endl; //debugging line
            partialMessage.append(reinterpret_cast<char*>(rxBuffer));

            // Handling complete messages
            size_t newlinePos = partialMessage.find('\n');
            if (newlinePos != std::string::npos) {
                std::string completeMessage = partialMessage.substr(0, newlinePos);
                std::cout << "Handling message: " << completeMessage << std::endl; //debugging line
                handleReceivedMessage(completeMessage, state); //processing the message
                partialMessage.erase(0, newlinePos + 1);
            }
        }
        
        

        //if error with recived
        else if (rxLength < 0) {
            perror("Read error");
            readErrorCount++;
            if (readErrorCount >= MAX_READ_ERRORS) {
                std::cerr << "Too many read errors, attempting to reset UART." << std::endl;
                if (!setupUART(uart0_filestream)) {
                    std::cerr << "Failed to reset UART. Exiting read loop." << std::endl;
                    break;
                }
                readErrorCount = 0;
            }
        }
        //no data received, but no error (e.g., non-blocking read)
        else {
            readErrorCount = 0; //reset error count as there's no error
        }
    }
}


//send command to open the door through UART
int sendOpenCommand(int uart0_filestream) {
    const char* command = "open_door\n";
    int length = strlen(command);
    int count = write(uart0_filestream, command, length);
    if (count < 0) {
        perror("UART TX error");
        return -1; //returning negative value to indicate error
    }
    return count;
}


bool setupUART(int& uart0_filestream) {
    //setupUART attempts
    constexpr int MAX_SETUP_ATTEMPTS = 5;
    constexpr int SETUP_RETRY_DELAY_MS = 500;

    for (int attempt = 0; attempt < MAX_SETUP_ATTEMPTS; ++attempt) {
        uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);

        if (uart0_filestream != -1) {
            //if open is successful, configure UART
            struct termios options;
            tcgetattr(uart0_filestream, &options);
            options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
            options.c_iflag = IGNPAR;
            options.c_oflag = 0;
            options.c_lflag = 0;
            tcflush(uart0_filestream, TCIFLUSH);
            if (tcsetattr(uart0_filestream, TCSANOW, &options) != -1) {
                return true;
            }
            close(uart0_filestream); //close if settings fail
        }

        std::cerr << "Attempt " << attempt + 1 << " to open UART failed." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(SETUP_RETRY_DELAY_MS));
    }

    return false;
}



//setup UART communication
// bool setupUART(int& uart0_filestream) {
//     uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
//     //uart0_filestream = open("/dev/pts/2", O_RDWR | O_NOCTTY | O_NDELAY);
//     if (uart0_filestream == -1) {
//         perror("Error - Unable to open UART");
//         return false;
//     }

//     struct termios options;
//     if (tcgetattr(uart0_filestream, &options) < 0) {
//         perror("Error - Could not get UART options");
//         close(uart0_filestream);
//         return false;
//     }
//     options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
//     options.c_iflag = IGNPAR;
//     options.c_oflag = 0;
//     options.c_lflag = 0;
//     tcflush(uart0_filestream, TCIFLUSH);
//     if (tcsetattr(uart0_filestream, TCSANOW, &options) < 0) {
//         perror("Error - Could not set UART options");
//         close(uart0_filestream);
//         return false;
//     }
//     return true;
// }

//handle messages received via UART
void handleReceivedMessage(const std::string& message, SystemState& state) {

    if (message == "door_closed\n") {
        state.doorIsClosed();
        std::cout << "Døren er lukket" << std::endl;
    }
    else if (message == "obstruction_detected\n") {
        std::cout << "Døren kan ikke lukke. Prøver igen om 10 sekunder";
    }
}