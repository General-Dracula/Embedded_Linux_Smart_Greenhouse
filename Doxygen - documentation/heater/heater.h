/**
    @author Mircea-Ionut Dobre  - 293117    
	@author Florina Toldea      - 299116    

    @file heater.h
    @brief Driver for using the heater

    @version 4.20                           
    @date 15/11/2022                        
*/
#ifndef GREENHOUSE_TEMP_HEATER_H
#define GREENHOUSE_TEMP_HEATER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>

class Heater
{
public:
    /// @brief Constructor for the heater
    Heater()
    {
        struct stat st;
        if (!stat("/sys/class/gpio/gpio50", &st) == 0)
        {
            // Create the file
            fs.open("/sys/class/gpio/export", std::fstream::out);
            fs << 50;
            fs.close();
        }

        // Set it to output
        fs.open("/sys/class/gpio/gpio50/direction", std::fstream::out);
        fs << "out";
        fs.close();
    }

    /// @brief Turn on the heater by turning on the GPIO pin 50
    void turnOnHeater();

    /// @brief Turn off the heater by turning on the GPIO pin 50
    void turnOffHeater();

private:
    /// @brief FSTREAM
    std::fstream fs;
};

#endif
