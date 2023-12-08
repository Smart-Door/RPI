//GPIOControl.h
#ifndef GPIOCONTROL_H
#define GPIOCONTROL_H


#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>


class GPIOControl {
public:

//funktion to read button state on GPIO 12 with driver
    static bool readButton();

//function to blink leds with correct open password
    static void blinkLEDs();

};

#endif //GPIOCONTROL_H
