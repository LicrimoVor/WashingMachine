#include "motor.h"
#include <Arduino.h>


Motor::Motor(int pin_motor, int pin_zero, int pin_tacho):
  pin_motor(pin_motor), pin_zero(pin_zero), pin_tacho(pin_tacho){
  pinMode(pin_tacho, INPUT);
  GyverPID regulator(0.1, 0.05, 0.01, 10);

  pinMode(pin_zero, INPUT);
  pinMode(pin_motor, OUTPUT);
  regulator.setDirection(NORMAL);  // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(100, 9500);  // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
}

void Motor::_isr_zero(){
  timmer_zero = micros();
  flag_alpha = 0;
  digitalWrite(pin_motor, 0);
}

void Motor::_isr_tacho(){
  tacho.tick();   // сообщаем библиотеке об этом
}

uint32_t Motor::get_speed(){
  return tacho.getRPM();
}


void Motor::set_speed(int speed_motor){
  regulator.setpoint = speed_motor * 138;
  regulator.input = get_speed();
  timmer_alpha = 10000 - regulator.getResultTimer();

  if (micros() - timmer_zero < 10000
      && micros() - timmer_zero > timmer_alpha
      && !flag_alpha
      ) {
    flag_alpha = 1;
    digitalWrite(pin_motor, 1);
    delayMicroseconds(10);
    digitalWrite(pin_motor, 0);
  }
}
