/**
    @author Mircea-Ionut Dobre  - 293117    
	@author Florina Toldea      - 299116    

    @file hih8120.h
    @brief Temperature and dumidity sensors driver

    @version 4.20                           
    @date 15/11/2022                        
*/
#include "../I2C_driver/I2CDevice.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <bitset>
#include <iomanip>

namespace greenhouse
{
    class hih8120 : protected exploringBB::I2CDevice
    {

    public:
        /// @brief I2C address for the sensor
        unsigned int i2xc;
        /// @brief Temperature in C
        float temperature;
        /// @brief Humidity
        float humidity;

    private:
        /// @brief I2C address for the sensor
        unsigned int I2CBus, I2CAddress;

        /// @brief Helper function
        /// @param value int  
        /// @return converted value as float
        float mapValueForHum(unsigned int value);
        /// @brief Helper function
        /// @param value int  
        /// @return converted value as float
        float maoValueForTemp(unsigned int value);

    public:
        /// @brief Initialize driver
        /// @param I2CBus 
        /// @param I2CAddress 
        hih8120(unsigned int I2CBus, unsigned int I2CAddress = 0x53);
        /// @brief Reads the temperature and humidity
        void readCurrentTemperatureAndHumidity();
    };

}
