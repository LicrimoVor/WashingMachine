#define PIN_MOTOR 4
#define PIN_ZERO 2

#include <GyverPID.h>
#include "tacho.h"

GyverPID regulator(0.1, 0.05, 0.01, 10);
uint32_t timmer_zero;
uint32_t timmer_alpha;
bool flag_alpha = 0;

void setup_motor() {
  attachInterrupt(0, zero_controller, CHANGE);
  pinMode(PIN_ZERO, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  regulator.setDirection(NORMAL);
  // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(100, 9500);
  // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
}

void zero_controller() {
  timmer_zero = micros();
  flag_alpha = 0;
  digitalWrite(PIN_MOTOR, 0);
}

void set_motor_speed(int speed_motor) {
  regulator.setpoint = speed_motor * 138;
  regulator.input = get_speed();
  timmer_alpha = 10000 - regulator.getResultTimer();

  if (micros() - timmer_zero < 10000
      && micros() - timmer_zero > timmer_alpha
      && !flag_alpha
      ) {
    flag_alpha = 1;
    digitalWrite(PIN_MOTOR, 1);
    delayMicroseconds(10);
    digitalWrite(PIN_MOTOR, 0);
  }
}

uint32_t get_timmer_alpha() {
  return timmer_alpha;
}
