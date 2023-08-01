#pragma once

#include "motor.h"
#include <Arduino.h>



struct Modes
{
    uint32_t temperature;
    uint32_t speed_rising;
    uint32_t time;
    int speed_washing = 00000000;
    Motor motor;

    Modes(Motor motor): motor(motor) {};
    void set_params(uint32_t temperature, int speed_rising, uint32_t time);
    void pre_washing();
    void main_washing();
    void rising();
private:
    void water_drain();
    void set_water();
};


