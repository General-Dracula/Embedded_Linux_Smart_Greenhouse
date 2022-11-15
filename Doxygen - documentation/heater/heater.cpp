/**
    @author Mircea-Ionut Dobre  - 293117    
	@author Florina Toldea      - 299116    

    @file heater.cpp
    @brief Driver for using the heater

    @version 4.20                           
    @date 15/11/2022                        
*/
#include "heater.h"

/// @brief Turn on the heater by turning on the GPIO pin 50
void Heater::turnOnHeater()
{
    fs.open("/sys/class/gpio/gpio50/value", std::fstream::out);
    fs << 1;
    fs.close();
}

/// @brief Turn off the heater by turning off the GPIO pin 50
void Heater::turnOffHeater()
{
    fs.open("/sys/class/gpio/gpio50/value", std::fstream::out);
    fs << 0;
    fs.close();
}


