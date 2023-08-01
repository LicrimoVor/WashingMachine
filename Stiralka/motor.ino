#define PIN_MOTOR 4
#define PIN_ZERO 2

#include <GyverPID.h>

GyverPID regulator(0.51, 3.38, 0.02);
uint32_t timmer_zero;
uint32_t timmer_alpha;
bool flag_alpha = 0;

void setup_motor() {
  attachInterrupt(0, zero_controller, CHANGE);
  pinMode(PIN_ZERO, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  regulator.setDirection(NORMAL);  // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(100, 9500);  // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
}

void zero_controller() {
  timmer_zero = micros();
  flag_alpha = 0;
  digitalWrite(PIN_MOTOR, 0);
}

void motor_work() {
//  regulator.setpoint = speed_motor * 138;
  regulator.input = get_speed();
  timmer_alpha = 9500 - regulator.getResultTimer();

  if (micros() - timmer_zero < 10000
      && micros() - timmer_zero > timmer_alpha
      && !flag_alpha
      ) {
    flag_alpha = 1;
    digitalWrite(PIN_MOTOR, 1);
    delayMicroseconds(10);
    digitalWrite(PIN_MOTOR, 0);
  }
//  return regulator.setpoint;
}

void parsing() {
  if (Serial.available() > 1) {
    
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
      case 'p': regulator.Kp = value; break;
      case 'i': regulator.Ki = value; break;
      case 'd': regulator.Kd = value; break;
      case 's': regulator.setpoint = value * 138;
                speed_motor=value;
        break;
      case 't': timmer_alpha = value; break;
    }
  }
}

uint32_t get_timmer_alpha() {
  return timmer_alpha;
}
