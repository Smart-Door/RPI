
#include "CommandHandler.h"


//verify if the provided password is correct
bool verifyPassword(SystemState& state, const std::string& password, bool isAdmin) {
    bool passwordCorrect = isAdmin ? checkAdminPassword(password) : checkUserPassword(password);
    if (!passwordCorrect) {
        state.failedPasswordAttempts++; //increments failed attempts counter
        if (state.failedPasswordAttempts >= 3) {
            state.isSystemLocked = true; //locks if 3 failed attempts
            std::cout << "System locked due to too many failed attempts." << std::endl;
        }
        return false;
    }

    state.failedPasswordAttempts = 0; //reset failed attempts counter
    return true;
}

//handle 'open' command to open the door
void handleOpenCommand(int uart0_filestream, SystemState& state) {

    //door not already open, within opening hours.
    if (!state.isDoorOpenOrOpening.load() && checkOpenTime(state)) {
        std::cout << "Døren åbnes - Inden for åbningstid." << std::endl;
        sendOpenCommand(uart0_filestream);
        state.doorIsOpen();
    }

    //door not already open, outside opening hours - password required
    else if (!state.isDoorOpenOrOpening.load() && !checkOpenTime(state))
    {
        std::cout << "Indtast admin eller user password for at åbne døren: ";
        std::string password;
        std::getline(std::cin, password);

        //user password entered
        if (verifyPassword(state, password, false))
        {
            std::cout << "Døren åbnes af user." << std::endl;
            blinkLEDs();
            sendOpenCommand(uart0_filestream);
            state.doorIsOpen();
        }

        //admin password entered
        else if (verifyPassword(state, password, true))
        {
            std::cout << "Døren åbnes af admin." << std::endl;
            blinkLEDs();
            sendOpenCommand(uart0_filestream);
            state.doorIsOpen();
        }

        //password entered is not user or admin (wrong)
        else
        {
            std::cout << "Forkert password." << std::endl;
        }
    }
    
    //If door is already open (flag is set)
    else
    {
        std::cout << "Døren er allerede åben eller åbner." << std::endl;
    }

}

//handle 'skift' command to change opening hours
/*
void handleSkiftCommand(SystemState& state) {


    std::cout << "Indtast admin password for at ændre åbningstider: ";
    std::string password;
    std::getline(std::cin, password);

    if (verifyPassword(state, password, true))
    {

        std::string day;
        int openHour, closeHour;

        std::cout << "Indtast dag (Mandag, Tirsdag, osv.): ";
        std::getline(std::cin, day);

        //if "dag" is entered correct(Mandag with capital M)
        if (state.validDays.find(day) != state.validDays.end())
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

            setOpeningHours(state, day, openHour, closeHour);
            std::cout << "Åbningstider ændret for " << day << std::endl;
        }

        //if "dag" is spelled incorrectly
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
*/

//handle 'skift' command to change opening hours
void handleSkiftCommand(SystemState& state) {
    std::cout << "Indtast admin password for at ændre åbningstider: ";
    std::string password;
    std::getline(std::cin, password);

    if (verifyPassword(state, password, true)) {
        std::string day;
        std::cout << "Indtast dag (Mandag, Tirsdag, osv.): ";
        std::getline(std::cin, day);

        if (state.validDays.find(day) == state.validDays.end()) {
            std::cout << "Ugyldig dag indtastet." << std::endl;
            return;
        }

        int openHour, closeHour;
        openHour = getValidHour("Indtast åbningstid (time, 0-23): ");
        closeHour = getValidHour("Indtast lukketid (time, 0-23): ");

        setOpeningHours(state, day, openHour, closeHour);
        std::cout << "Åbningstider ændret for " << day << std::endl;
    } else {
        std::cout << "Forkert password." << std::endl;
    }
}

//check valid hour
int getValidHour(const std::string& prompt) {
    int hour;
    std::cout << prompt;
    while (!(std::cin >> hour) || hour < 0 || hour > 23) {
        std::cout << "Ugyldig time, prøv igen: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore();
    return hour;
}

//main loop to process user commands
void runCommandLoop(int uart0_filestream, SystemState& state) {
    
    
    while (true) {

        //handle system lock before processing commands
        if (state.isSystemLocked.load()) {
            std::cout << "Systemet er låst. Indtast admin password ";
            std::string password;
            std::getline(std::cin, password);
            if (!checkAdminPassword(password)) {
                std::cout << "Forkert password." << std::endl;
                continue;
            }
            state.failedPasswordAttempts = 0;
            state.isSystemLocked = false;
        }


        std::cout << "Kommandoer: 'open', 'vis', 'skift', 'exit'" << std::endl;
        std::cout << "Indtast kommando: ";
        std::string command;
        std::getline(std::cin, command);

        //command = open
        if (command == "open")
        {
            handleOpenCommand(uart0_filestream, state);
        }

        //command = vis
        else if (command == "vis")
        {
            printOpeningHours(state);
        }

        //command = skift
        else if (command == "skift")
        {
            handleSkiftCommand(state);
        }

        //command = exit
        else if (command == "exit")
        {
            break;
        }
        //Command isnt any of the possibilities
        else
        {
            std::cout << "Ugyldig kommando." << std::endl;
        }
    }
}
