//
// Created by jespe on 29-11-2023.
//

#include "GPIOControl.h"


//funktion to read button state on GPIO 12 with driver
bool readButton() {
    std::ifstream valueFile("/dev/plat_drv_12");
    //std::ifstream valueFile("/dev/pts/3");
    if (!valueFile) {
        std::cerr << "Failed to open /dev/plat_drv_12 for reading." << std::endl;
        return false;
    }
    int value;
    valueFile >> value;
    return value == 0;
}

//function to blink leds with correct open password
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

//Test funktion
// void blinkLEDs() {
//     std::cout << "LED ON" << std::endl;
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     std::cout << "LED OFF" << std::endl;
//     std::this_thread::sleep_for(std::chrono::milliseconds(500));

// }