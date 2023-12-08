//GPIOControl.cpp
#include "GPIOControl.h"


//funktion to read button state on GPIO 12 with driver
bool GPIOControl::readButton() {
    //std::ifstream valueFile("/dev/plat_drv_12");
    std::ifstream valueFile("/dev/mygpio0");
    //std::ifstream valueFile("/dev/pts/3");
    if (!valueFile) {
        //std::cerr << "Failed to open /dev/plat_drv_12 for reading." << std::endl;
        std::cerr << "Failed to open /dev/mygpio0 for reading." << std::endl;
        return false;
    }
    int value;
    valueFile >> value;
    return value == 0;
}

//function to blink leds with correct open password
void GPIOControl::blinkLEDs() {
    //const std::string leds[] = {"/dev/plat_drv_20", "/dev/plat_drv_21", "/dev/plat_drv_26"};
    const std::string leds[] = {"/dev/mygpio2", "/dev/mygpio3", "/dev/mygpio4"};
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
