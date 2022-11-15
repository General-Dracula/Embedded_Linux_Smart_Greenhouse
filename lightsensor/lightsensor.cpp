/**
    @author Mircea-Ionut Dobre  - 293117
    @author Florina Toldea      - 299116

    @file lightsensor.cpp
    @brief Driver for using the lightsensor

    @version 4.20
    @date 15/11/2022
*/
#include "lightsensor.h"

using namespace std;

namespace greenhouse
{
    /// @brief Constructor
    /// @param I2CBus int address
    lightsensor::lightsensor(unsigned int I2CBus)
    {
        this->lightLevel = I2CBus;
    }

    /// @brief Read the current light level
    void lightsensor::readCurrentLightLevel()
    {
        ifstream indata;
        char data[4];
        indata.open("/sys/bus/iio/devices/iio\:device0/in_voltage0_raw");
        indata >> data;
        indata.close();
        int value = atoi(data);
        this->lightLevel = this->convertVoltsToPercentage(value);
    }

    /// @brief Convert the light value to percentage
    /// @param value int
    /// @return float Converted value
    float lightsensor::convertVoltsToPercentage(unsigned int value)
    {
        int min = 0;
        float max = 3700;
        return floor((value / max * 100.0) * 10) / 10;
    }
}