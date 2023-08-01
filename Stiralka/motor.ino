#define PIN_MOTOR 4
#define PIN_ZERO 2

#include <GyverPID.h>
#include <GyverTimers.h>

GyverPID regulator(0.51, 3.38, 0.02, 100);
// uint32_t timmer_zero;
uint32_t timmer_alpha;
// bool flag_alpha = 0;

void setup_motor() {
  attachInterrupt(0, zero_controller, CHANGE);
  pinMode(PIN_ZERO, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  regulator.setDirection(NORMAL);  // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(100, 9500);  // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  Timer2.enableISR();
}

void zero_controller() {
  static int lastDim;
  // timmer_zero = micros();
  digitalWrite(PIN_MOTOR, 0); // выключаем симистор
  // если значение изменилось, устанавливаем новый период
  // если нет, то просто перезапускаем со старым
  if (lastDim != timmer_alpha) Timer2.setPeriod(lastDim = timmer_alpha);
  else Timer2.restart();
}

// прерывание таймера
ISR(TIMER2_A) {
  digitalWrite(PIN_MOTOR, 1);  // включаем симистор
  Timer2.stop();                // останавливаем таймер
}

void motor_work() {
//  regulator.setpoint = speed_motor * 138;
  regulator.input = get_speed();
  timmer_alpha = 9500 - regulator.getResultTimer();

  // if (micros() - timmer_zero < 10000
  //     && micros() - timmer_zero > timmer_alpha
  //     && !flag_alpha
  //     ) {
  //   flag_alpha = 1;
  //   digitalWrite(PIN_MOTOR, 1);
  //   delayMicroseconds(10);
  //   digitalWrite(PIN_MOTOR, 0);
  // }
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
