#pragma once

#include <Arduino.h>
#include <GyverPID.h>
#include <Tachometer.h>

struct Motor
{
    uint32_t timmer_alpha;
    uint32_t get_speed();
    void set_speed(int speed_motor);
    Motor(int pin_motor, int pin_zero, int pin_tacho);
    void _isr_tacho();
    void _isr_zero();
private:
    int pin_motor;
    int pin_zero;
    int pin_tacho;
    uint32_t timmer_zero;
    GyverPID regulator;
    Tachometer tacho;
    bool flag_alpha = 0;
};

#include "realization/motor.cpp" // WTF?!?!?!??!?!?!?!?
