//main.cpp
#include <iostream>
#include <exception>
#include <thread>

#include "SerialCommunication.h"
#include "TimeUtility.h"
#include "CommandHandler.h"
#include "SystemState.h"
#include "PasswordManager.h"



//function to start a thread monitoring button press for opening the door
std::thread startButtonThread(SystemState& state, SerialCommunication& serialComm) {
    return std::thread([&state, &serialComm]() {
        while (state.programRunning.load()) {
            if (SerialCommunication::readButton() && !state.isDoorOpenOrOpening.load()) {
                std::cout << "Døren åbnes - med knap" << std::endl;
                serialComm.sendOpenCommand();
                state.doorIsOpen();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
    });
}

//function to start a thread for reading commands from UART
std::thread startReadThread(SystemState& state, SerialCommunication& serialComm) {
    return std::thread([&state, &serialComm]() {
        serialComm.readCommand(state);
    });
}

int main() {


    try {
        SystemState state;
        SerialCommunication serialComm;
        TimeUtility timeUtility;


        //setup UART communication
        if (!serialComm.setupUART()) {
            return 1;
        }

        //start threads for button monitoring and UART reading
        std::thread buttonThread = startButtonThread(state, serialComm);
        std::thread readThread = startReadThread(state, serialComm);

        //load credentials, users.txt
        auto credentials = PasswordManager::readUserCredentials("users.txt");

        //create and run command loop which is the program
        CommandHandler commandHandler(state, serialComm, timeUtility);
        commandHandler.runCommandLoop(credentials);

        //shutdown sequence
        state.programRunning = false;
        buttonThread.join();
        readThread.join();



    //catch and display exceptions    
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
