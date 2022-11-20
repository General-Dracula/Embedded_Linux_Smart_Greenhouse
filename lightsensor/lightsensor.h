/**
    @author Mircea-Ionut Dobre  - 293117
    @author Florina Toldea      - 299116

    @file lightsensor.h
    @brief Driver for using the lightsensor

    @version 4.20
    @date 15/11/2022
*/
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <bitset>
#include <iomanip>
#include <fstream>

namespace greenhouse
{
    class lightsensor
    {
    public:
        /// @brief Light Level
        float lightLevel;

    private:
        /// @brief Addresses
        unsigned int I2CBus, I2CAddress;

        /// @brief Convert the light value to percentage
        /// @param value int
        /// @return float Converted value
        float convertVoltsToPercentage(unsigned int value);

    public:
        /// @brief Constructor
        /// @param I2CBus int address
        lightsensor(unsigned int I2CBus);
        /// @brief Read the current light level
        void readCurrentLightLevel();
    };
}
