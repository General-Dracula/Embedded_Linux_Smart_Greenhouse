/**
    @author Mircea-Ionut Dobre  - 293117    
	@author Florina Toldea      - 299116    

    @file main.cpp
    @brief Main function of our program used by the node script server

    @version 4.20                           
    @date 15/11/2022                        
*/
#include "temperature_humidity/hih8120.h"
#include "lightsensor/lightsensor.h"
#include "pwm/pwm.h"
#include "heater/heater.h"
#include <iostream>

/// @brief HIH8120 I2C address
#define ADDR 0x27 
/// @brief Servo channel
#define SERVOCHANNEL 0
/// @brief Light channel
#define LIGHTCHANNEL 1
/// @brief Heater port
#define HEATER 50



/**
    \brief Prints the temperature and humidity from the hih8180 sensor
*/
void printTemperatureAndHumidity()
{
    greenhouse::hih8120 hih8120(2, HIH8120_ADDR);
    hih8120.readCurrentTemperatureAndHumidity();
    std::cout << "---Current temperature and himidity by HIH8120---" << std::endl;
    std::cout << "-Temperature: " << hih8120.temperature << "     " << std::endl;
    std::cout << "-Humidity:    " << hih8120.humidity << "     " << std::endl;
}

/**
    \brief Prints only the temperature from the hih8180 sensor
*/
void printTemperature()
{
    greenhouse::hih8120 hih8120(2, HIH8120_ADDR);
    hih8120.readCurrentTemperatureAndHumidity();
    std::cout << "---Current temperature by HIH8120---" << std::endl;
    std::cout << "-Temperature: " << hih8120.temperature << "     " << std::endl;
}

/**
    \brief Prints only the humidity from the hih8180 sensor
*/
void printHumidity()
{
    greenhouse::hih8120 hih8120(2, HIH8120_ADDR);
    hih8120.readCurrentTemperatureAndHumidity();
    std::cout << "Humidity:    " << hih8120.humidity << "     " << std::endl;
}

/**
    \brief Control the light internsity by giving it a % value from 0 to 100
    \param intensity 0-100 integer value
*/
void controlLightIntensity(int intensity)
{
    pwm pwmLed;
    pwmLed.setPwmByPercentForLight(intensity, 20000000, LIGHTCHANNEL);
}

/**
    \brief Control the window on or off by oen string which can be either open or close
    \param status "open" / "close" string value
*/
void controlServo(std::string status)
{
    pwm pwmServo;

    if (status == "open")
    {
        pwmServo.sendPwm(2000000, 20000000, SERVOCHANNEL);
    }
    else if (status == "close")
    {
        pwmServo.sendPwm(1000000, 20000000, SERVOCHANNEL);
    }
}


/**
    \brief Function to read text from files
    \param path Path to the file
    \return std::string Returns the text from the file
 */
std::string readFile(std::string path)
{
    std::ifstream fs;
    std::string data;
    fs.open(path);
    fs >> data;
    fs.close();
    return data;
}

/**
    \brief Turn on or off the heater depending on the value received
    \param status "on" / "off" string value
*/
void heaterOnOff(std::string status)
{
    Heater heater;
    if (status == "on")
    {
        heater.turnOnHeater();
    }
    else if (status == "off")
    {
        heater.turnOffHeater();
    }
}

/**
    \brief Prints the current light level of where it is placed by using the photoresistor
*/
void printLightLevel()
{
    greenhouse::photoresistor photoresistor(2);
    photoresistor.readCurrentLightLevel();
    std::cout << "Light level: " << photoresistor.lightLevel << "     " << std::endl;
}

/**
    \brief Prints the current window status 2000000 = open | 1000000 = closed
*/
void readWindowPos()
{
    std::string wpos = readFile("/sys/class/pwm/pwmchip1/pwm-1:0/duty_cycle");
    if (wpos.compare("2000000") == 0)
    {
        std::cout << "Window:      open"
                  << "     " << std::endl;
    }
    else if (wpos.compare("1000000") == 0)
    {
        std::cout << "Window:      closed"
                  << "     " << std::endl;
    }
    else
    {
        std::cout << "error reading window pos" << std::endl;
    }
}

/**
    \brief Prints the current heater status position 1 = on | 0 = off
*/
void readHeater()
{
    struct stat st;
    if (stat("/sys/class/gpio/gpio50", &st) == 0)
    {
        std::string val = readFile("/sys/class/gpio/gpio50/value");
        if (val[0] == '1')
        {
            std::cout << "Heater:      on"
                      << "    " << std::endl;
        }
        else
        {
            std::cout << "Heater:      off"
                      << "    " << std::endl;
        }
    }
    else
    {
        std::cout << "Heater:      off"
                  << "    " << std::endl;
    }
}

/**
    \brief Prints the current light intensity in % 0-100
*/
void readLightIntensity()
{
    std::string lightDuty = readFile("/sys/class/pwm/pwmchip1/pwm-1:1/duty_cycle");
    std::cout << "Led light:   " + std::to_string(atoi(lightDuty.c_str()) / 200000) << "     " << std::endl;
}

/**
    \brief This is the main function of the program
    \param argc int Command code
    \param argv char Arguments given
*/
int main(int argc, char **argv)
{
    if (argc >= 2)
    {
        if (std::string(argv[1]) == "--help")
        {

            std::cout << std::endl
                      << "Greenhouse assignment options: " << std::endl
                      << std::endl
                      << "readTempAndHumidity - Prints current temperature and humidity" << std::endl
                      << "readTemp - Prints current temperature" << std::endl
                      << "readHumidity - Prints current humidity" << std::endl
                      << "readLightLevel - Prints current light level" << std::endl
                      << "setLedLight - controls led light intensity 0 - 100" << std::endl
                      << "readLedLight - Prints led light intensity" << std::endl
                      << "setWindowStatus - controls window open/close" << std::endl
                      << "readWindow - prints postition open/close" << std::endl
                      << "readHeater - prints status on/off" << std::endl
                      << "setHeaterStatus - controls heater on/off" << std::endl;
        }
        else if (std::string(argv[1]) == "readTempAndHumidity")
        {

            printTemperatureAndHumidity();
        }
        else if (std::string(argv[1]) == "readTemp")
        {

            printTemperature();
        }
        else if (std::string(argv[1]) == "readHumidity")
        {

            printHumidity();
        }
        else if (std::string(argv[1]) == "readLightLevel")
        {

            printLightLevel();
        }
        else if (std::string(argv[1]) == "setLedLight")
        {

            controlLightIntensity(atoi(argv[2]));
        }
        else if (std::string(argv[1]) == "readLedLight")
        {

            readLightIntensity();
        }
        else if (std::string(argv[1]) == "setWindowStatus")
        {

            controlServo(argv[2]);
        }
        else if (std::string(argv[1]) == "readWindow")
        {

            readWindowPos();
        }
        else if (std::string(argv[1]) == "setHeaterStatus")
        {

            heaterOnOff(argv[2]);
        }
        else if (std::string(argv[1]) == "readHeater")
        {

            readHeater();
        }
        else
        {

            std::cout << "Argument unrecognized see --help" << std::endl;
        }
    }
    else
    {

        std::cout << "Wrong argumments use: --help" << std::endl;
    }

    return 0;
}