/**
    @author Mircea-Ionut Dobre  - 293117    
	@author Florina Toldea      - 299116    

    @file pwm.cpp
    @brief Pwm driver

    @version 4.20                           
    @date 15/11/2022                        
*/
#include "pwm.h"

/// @brief Change the PWM values
/// @param duty_cycle int
/// @param period int
/// @param channel int
void pwm::sendPwm(int duty_cycle, int period, int channel)
{

    fs.open(("/sys/class/pwm/pwmchip1/pwm-1:" + std::to_string(channel) + "/duty_cycle").c_str(), std::fstream::out);
    fs << std::to_string(duty_cycle);
    fs.close();

    fs.open(("/sys/class/pwm/pwmchip1/pwm-1:" + std::to_string(channel) + "/period").c_str(), std::fstream::out);
    fs << std::to_string(period);
    fs.close();

    fs.open(("/sys/class/pwm/pwmchip1/pwm-1:" + std::to_string(channel) + "/enable").c_str(), std::fstream::out);
    fs << std::to_string(1);
    fs.close();
}

/// @brief Set the light value by percent
/// @param perc int     
/// @param period int
/// @param channel int
void pwm::setPwmByPercentForLight(int perc, int period, int channel)
{
    sendPwm(perc * 200000, period, channel);
}