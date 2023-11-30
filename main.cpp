#include <iostream>
#include <exception>
#include <thread>

#include "SerialCommunication.h"
#include "AccessControl.h"
#include "CommandHandler.h"
#include "SystemState.h"
#include "GPIOControl.h"

constexpr char openDoorCommand[] = "open_door";


//function to start a thread monitoring button press for opening the door
std::thread startButtonThread(SystemState& state, int& uart0_filestream) {
    return std::thread([&state, &uart0_filestream]() {
        while (state.programRunning.load()) {
            bool buttonPressed = readButton();
            if (buttonPressed && !state.isDoorOpenOrOpening.load()) {
                std::cout << "Døren åbnes - med knap" << std::endl;
                sendOpenCommand(uart0_filestream);
                state.isDoorOpenOrOpening = true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
    });
}

//function to start a thread for reading commands from UART
std::thread startReadThread(SystemState& state, int& uart0_filestream) {
    return std::thread([&state, &uart0_filestream]() {
        readCommand(uart0_filestream, state); // Modify readCommand to accept state
    });
}

int main() {
    SystemState state; //nitialize system state

    try {
        int uart0_filestream = -1;

        //setup UART communication
        if (!setupUART(uart0_filestream)) {
            return 1;
        }

        //start threads for button monitoring and UART reading
        std::thread buttonThread = startButtonThread(state, uart0_filestream);
        std::thread readThread = startReadThread(state, uart0_filestream);

        //run command loop for handling user input
        runCommandLoop(uart0_filestream, state);

        //shutdown sequence
        state.programRunning = false;
        buttonThread.join();
        readThread.join();

        //close UART filestream if it's open
        if (uart0_filestream != -1) {
            close(uart0_filestream);
        }

    //catch and display exceptions    
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
