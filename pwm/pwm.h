/**
    @author Mircea-Ionut Dobre  - 293117    
	@author Florina Toldea      - 299116    

    @file pwm.h
    @brief Pwm driver

    @version 4.20                           
    @date 15/11/2022                        
*/

#ifndef BEAGLEBONEGREENHOUSE_PWM_H
#define BEAGLEBONEGREENHOUSE_PWM_H
#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>

class pwm {
public:
    /// @brief Constructor for initializing PWM servo and LEDS
    pwm()
    {
        system("config-pin P9_22 pwm");
        system("config-pin P9_21 pwm");
    }

    /// @brief Change the PWM values
    /// @param duty_cycle int
    /// @param period int
    /// @param channel int
    void sendPwm(int duty_cycle, int period, int channel);
    /// @brief Set the light value by percent
    /// @param perc int
    /// @param period int
    /// @param channel int
    void setPwmByPercentForLight(int perc, int period, int channel);

private:
    std::fstream fs;
};




#endif
