#pragma once

#include "motor.h"
#include <Arduino.h>



struct Modes
{
    uint16_t temperature;
    uint16_t speed_rising;
    uint16_t time;
    uint16_t speed_washing = "???";

    Modes(uint16_t temperature, uint16_t speed_rising, uint16_t time);

    void pre_washing();
    void main_washing();
    void rising();
private:
    void water_drain();
    void set_water();
};


