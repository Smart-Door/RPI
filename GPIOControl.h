//
// Created by jespe on 29-11-2023.
//

#ifndef GPIOCONTROL_H
#define GPIOCONTROL_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

//funktion to read button state on GPIO 12 with driver
bool readButton();
//function to blink leds with correct open password
void blinkLEDs();

#endif //PROJEKT_TEST_1_NY_GPIOCONTROL_H
