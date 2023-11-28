#include <iostream>

#include <set>
#include <thread>
#include <limits>
#include <fstream>
#include <string>
#include <chrono>
#include <exception>

#include "SerialCommunication.h"
#include "AccessControl.h"

#define openDoorCommand "open_door"

//BOOL FLAG
std::atomic<bool> isDoorOpenOrOpening{false}; //Om døren åbner eller er åben
std::atomic<bool> programRunning{true}; //om programmet kører


std::set<std::string> validDays = {"Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag", "Søndag"};



//Funktion til at læse knap GPIO på 12
bool readButton() {
    std::ifstream valueFile("/dev/plat_drv_12");
    if (!valueFile.is_open()) {
        std::cerr << "Failed to open /dev/plat_drv_12 for reading." << std::endl;
        return false;
    }

    int value;
    valueFile >> value;
    valueFile.close();
    return value == 0;
}

void blinkLEDs() {
    const std::string leds[] = {"/dev/plat_drv_20", "/dev/plat_drv_21", "/dev/plat_drv_26"};
    for (int i = 0; i < 3; ++i) {
        for (const auto& led : leds) {
            std::ofstream(led) << "1";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        for (const auto& led : leds) {
            std::ofstream(led) << "0";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}



int main() {
    try {
        int uart0_filestream = -1;

        // Open the UART
        uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
        if (uart0_filestream == -1) {
            printf("Error - Unable to open UART. Ensure it is not in use by another application\n");
            return 1;
        }
        

        // Configure the UART
        struct termios options;
        tcgetattr(uart0_filestream, &options);
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(uart0_filestream, TCIFLUSH);
        tcsetattr(uart0_filestream, TCSANOW, &options);

    //Initierer variabler
    std::string command, password, day;
    int openHour, closeHour;

    //GPIO KNAP
std::thread buttonThread([&uart0_filestream]() {
    while (programRunning.load()) {
        bool buttonPressed = readButton();
        if (buttonPressed && !isDoorOpenOrOpening.load()) {
            std::cout << "Døren åbnes - med knap" << std::endl;
            sendOpenCommand(uart0_filestream);
            isDoorOpenOrOpening = true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
});





    while (programRunning.load()) { //While loop til at sikre threads lukkes

        //Tråd til at læse indkommende commands
        std::thread readThread(readCommand, uart0_filestream);
        //Selve menuen
        while (true) 
        {
            std::cout << "Kommandoer: 'open', 'vis', 'skift', 'exit'" << std::endl;
            std::cout << "Indtast kommando: ";
            std::getline(std::cin, command);

//command = open
            if (command == "open") 
            {
                
                //inden for åbningstid. Døren er ikke allerede åben.
                if (!isDoorOpenOrOpening.load() && checkOpenTime()) 
                {
                    std::cout << "Døren åbnes - Inden for åbningstid." << std::endl;
                    sendOpenCommand(uart0_filestream);
                    isDoorOpenOrOpening = true;
                }

                //Uden for åbningstid. Døren er ikke allerede allerede åben
                else if (!isDoorOpenOrOpening.load()) 
                {
                    std::cout << "Indtast admin eller user password for at åbne døren: ";
                    std::getline(std::cin, password);

                    //admin password indtastes
                    if (checkAdminPassword(password)) 
                    {
                        std::cout << "Døren åbnes af admin." << std::endl;
                        sendOpenCommand(uart0_filestream);
                        isDoorOpenOrOpening = true;
                    }

                    //user password indtastet
                    else if (checkUserPassword(password)) 
                    {
                        std::cout << "Døren åbnes af bruger." << std::endl;
                        sendOpenCommand(uart0_filestream);
                        isDoorOpenOrOpening = true;
                    }

                    //password er hverken user eller admin password
                    else 
                    {
                        std::cout << "Forkert password." << std::endl;
                    }
                }

                //Hvis døren allerede er åben (flaget isDoorOpenOrOpening er sat true)
                else
                {
                    std::cout << "Døren er allerede åben eller åbner." << std::endl;
                }

            }

//command = vis
            else if (command == "vis")
            {
                printOpeningHours();
            }

//command = skift
            else if (command == "skift")
            {
                std::cout << "Indtast admin password for at ændre åbningstider: ";
                std::getline(std::cin, password);

                if (checkAdminPassword(password))
                {
                    std::cout << "Indtast dag (Mandag, Tirsdag, osv.): ";
                    std::getline(std::cin, day);
                    
                    //hvis dag er indtastet korrekt(Mandag - med stort forbogstav)
                    if (validDays.find(day) != validDays.end())
                    {
                        std::cout << "Indtast åbningstid (time, 0-23): ";
                        while (!(std::cin >> openHour) || openHour < 0 || openHour > 23)
                        {
                            std::cout << "Ugyldig time, prøv igen: ";
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }

                        std::cin.ignore();
                        std::cout << "Indtast lukketid (time, 0-23): ";

                        while (!(std::cin >> closeHour) || closeHour < 0 || closeHour > 23)
                        {
                            std::cout << "Ugyldig time, prøv igen: ";
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                        std::cin.ignore();

                        setOpeningHours(day, openHour, closeHour);
                        std::cout << "Åbningstider ændret for " << day << std::endl;
                    }
                    //Hvis dag er stavet forkert
                    else
                    {
                        std::cout << "Ugyldig dag indtastet." << std::endl;
                    }
                }
                //Hvis admin password er forkert
                else
                {
                    std::cout << "Forkert password." << std::endl;
                }

            }
//command = exit
            else if (command == "exit")
            {
                break;
            }
//Command svarer ikke til nogen af valgmulighederne
            else
            {
                std::cout << "Ugyldig kommando." << std::endl;
            }
        }
    }
        
    
        
 
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
//NY LUKKER THREADS
    programRunning = false;
    buttonThread.join();
    readThread.join();

//NY    //Lukker UART
    if (uart0_filestream != -1) {
        close(uart0_filestream);
    }
    return 0;
}
