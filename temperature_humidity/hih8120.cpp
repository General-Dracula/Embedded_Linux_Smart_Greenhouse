/**
    @author Mircea-Ionut Dobre  - 293117
    @author Florina Toldea      - 299116

    @file hih8120.cpp
    @brief Temperature and humidy sensors driver

    @version 4.20
    @date 15/11/2022
*/
#include "hih8120.h"

using namespace std;

namespace greenhouse
{
    /**
        \brief Initialize the sensor values
        \param I2CBus int
        \param I2CAdress int
    */
    hih8120::hih8120(unsigned int I2CBus, unsigned int I2CAddress) : I2CDevice(I2CBus, I2CAddress)
    {
        this->I2CAddress = I2CAddress;
        this->I2CBus = I2CBus;

        this->temperature = -100;
        this->humidity = -100;
    }

    /**
        \brief Reads the temperature and humidity
    */
    void hih8120::readCurrentTemperatureAndHumidity()
    {
        unsigned char *response = this->readDevice(4);

        if (response[0] > 127)
        {
            cout << "Bad status" << std::endl;
        }

        int adjustedRes0 = response[0];

        if (response[0] > 63)
        {
            adjustedRes0 = response[0] & ~(0x01 << 6);
        }

        unsigned int humidity = adjustedRes0 << 8 | response[1];

        unsigned int temperature = response[2] << 6 | response[3] >> 2;

        this->humidity = this->mapValueForHum(humidity);
        this->temperature = this->maoValueForTemp(temperature);
    }

    /**
        \brief Mapping the values for humidity
        \param value int 
        \return converted value as float
    */
    float hih8120::mapValueForHum(unsigned int value)
    {
        return floor((value / 16383.0 * 100.0) * 10) / 10;
    }

    /**
        \brief Mapping the value for temperature
        \param value int 
        \return converted value as float
    */
    float hih8120::maoValueForTemp(unsigned int value)
    {
        return floor((value / 16383.0 * 165.0 - 40.0) * 10) / 10;
    }
}