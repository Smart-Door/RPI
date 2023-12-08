//CommandHandler.cpp
#include "CommandHandler.h"
#include "GPIOControl.h"

CommandHandler::CommandHandler(SystemState& state, SerialCommunication& serialComm, TimeUtility& timeUtility)
        : state(state), serialComm(serialComm), timeUtility(timeUtility) {}

//main loop to process user commands
void CommandHandler::runCommandLoop(const std::map<std::string, std::string>& credentials) {
    while (true) {
        if (state.isSystemLocked) {
            if (!PasswordManager::unlockSystem(credentials, state)) {
                continue;
            }
        }

        std::cout << "*** Kommandoer ***:\n"
                  << "  1      - Åben dør\n"
                  << "  2      - Vis åbningstider\n"
                  << "  3      - Skift åbningstider\n"
                  << "  4      - help\n"
                  << "  CTRL+C - Afslut program\n";
        std::cout << "  Vælg 1 - 4 ";




        std::string MenuInput;
        std::getline(std::cin, MenuInput);

        std::stringstream ss(MenuInput);
        int MenuChoice;
        if (!(ss >> MenuChoice) || !(MenuChoice >= 1 && MenuChoice <= 5)) {
            std::cout << "------------------------------\n";
            std::cout << "Ugyldig valg. Prøv igen.\n";
            std::cout << "------------------------------\n";

            continue;
        }


        //switch for the menu choices
        switch (MenuChoice) {
            case 1:
                handleOpenCommand(credentials);
                break;
            case 2:
                timeUtility.printOpeningHours();
                break;
            case 3:
                handleSkiftCommand(credentials);
                break;
            case 4:
                std::cout << "------------------------------\n";
                std::cout << "Døren kan åbnes uden login\n";
                std::cout << "inden for åbningstid.\n";
                std::cout << "\n";
                std::cout << "Ønsker du adgang?\n";
                std::cout << "Kontakt   88888888?\n";
                std::cout << "\n";
                std::cout << "Har du glemt dit login?\n";
                std::cout << "Kontakt teknik: 98988989\n";
                std::cout << "\n";
                std::cout << "Afslut program med CTRL+C\n";
                std::cout << "------------------------------\n";

                break;
            default:
                std::cout << "------------------------------\n";
                std::cout << "Ugyldig valg. Prøv igen.\n";
                std::cout << "------------------------------\n";

                break;
        }
    }
}



//handle 'open' command to open the door

void CommandHandler::handleOpenCommand(const std::map<std::string, std::string>& credentials) {
    //if the door is not open and it's within opening hours
    if (!state.isDoorOpenOrOpening.load() && timeUtility.checkOpenTime()) {
        std::cout << "------------------------------\n";
        std::cout << "Døren åbnes - Inden for åbningstid." << std::endl;
        std::cout << "------------------------------\n";


        serialComm.sendOpenCommand();
        state.doorIsOpen();
    }
        //if the door is not open and it's outside opening hours, password required
    else if (!state.isDoorOpenOrOpening.load() && !timeUtility.checkOpenTime()) {
        if (PasswordManager::authenticateOpenCommand(credentials, state)) {

            std::cout << "------------------------------\n";
            std::cout << "Døren åbnes efter godkendelse." << std::endl;
            std::cout << "------------------------------\n";

            serialComm.sendOpenCommand();
            GPIOControl::blinkLEDs();
            state.doorIsOpen();
        } else {
            std::cout << "------------------------------\n";
            std::cout << "Forkert password." << std::endl;
            std::cout << "------------------------------\n";

        }
    }
        //if the door is already open
    else {
        std::cout << "------------------------------\n";
        std::cout << "Døren er allerede åben eller åbner." << std::endl;
        std::cout << "------------------------------\n";

    }
}




//handle 'skift' command to change opening hours
void CommandHandler::handleSkiftCommand(const std::map<std::string, std::string>& credentials) {
    std::cout << "------------------------------\n";
    std::cout << "Admin login kræves for at åbningstider: ";
    std::cout << "\n------------------------------\n";


    if (PasswordManager::authenticateSkiftCommand(credentials, state)) {

        while (true) {

            std::cout << "\nVælg en dag:\n"
                      << "1 - Mandag\n"
                      << "2 - Tirsdag\n"
                      << "3 - Onsdag\n"
                      << "4 - Torsdag\n"
                      << "5 - Fredag\n"
                      << "6 - Lørdag\n"
                      << "7 - Søndag\n";
            std::cout << "Vælg en dag: ";

            std::string DayInput;
            std::getline(std::cin, DayInput);

            std::stringstream ss(DayInput);
            int DayChoice;
            if (!(ss >> DayChoice) || !(DayChoice >= 1 && DayChoice <= 5)) {
                std::cout << "------------------------------\n";
                std::cout << "Ugyldig valg. Prøv igen.\n";
                std::cout << "------------------------------\n";

                continue;
            }
            //switch for the day choices
            std::string day;
            switch (DayChoice) {
                case 1:
                    day = "Mandag";
                    break;
                case 2:
                    day = "Tirsdag";
                    break;
                case 3:
                    day = "Onsdag";
                    break;
                case 4:
                    day = "Torsdag";
                    break;
                case 5:
                    day = "Fredag";
                    break;
                case 6:
                    day = "Lørdag";
                    break;
                case 7:
                    day = "Søndag";
                    break;

                default:
                    std::cout << "------------------------------\n";
                    std::cout << "Ugyldigt valg af dag..." << std::endl;
                    std::cout << "------------------------------\n";

                    return;
            }

            if (day.empty()) {
                std::cout << "------------------------------\n";
                std::cout << "Ingen dag valgt. Afslutter ændring af åbningstider." << std::endl;
                std::cout << "------------------------------\n";

                return;
            }


            int openHour = TimeUtility::getValidHour("Indtast åbningstid (time, 0-23): ");
            int closeHour = TimeUtility::getValidHour("Indtast lukketid (time, 0-23): ");


            timeUtility.setOpeningHours(day, openHour, closeHour);
            std::cout << "------------------------------\n";
            std::cout << "Åbningstider ændret for " << day << std::endl;
            std::cout << "------------------------------\n";

            return;
        }

    } else {
        std::cout << "------------------------------\n";
        std::cout << "Forkert password." << std::endl;
        std::cout << "------------------------------\n";

    }
}






