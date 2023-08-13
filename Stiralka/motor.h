#include <Arduino.h>
#include <GyverPID.h>
#include <GyverTimers.h>
#include <GyverMotor.h>
#include "tachometer.h"

uint8_t pin_motor;
uint32_t timmer_alpha;

void zero_controller() {
  static int lastDim;
  digitalWrite(pin_motor, 0); // выключаем симистор
  // если значение изменилось, устанавливаем новый период
  // если нет, то просто перезапускаем со старым
  if (lastDim != timmer_alpha) Timer2.setPeriod(lastDim = timmer_alpha);
  else Timer2.restart();
}

// прерывание таймера
ISR(TIMER2_A) {
  digitalWrite(pin_motor, 1);  // включаем симистор
  Timer2.stop();                // останавливаем таймер
}


struct Motor{
  Motor(uint8_t pin_motor_, uint8_t pin_zero, uint8_t pin_deriction_1, uint8_t pin_deriction_2, uint8_t numb_interapt):
    pin_zero(pin_zero), pin_deriction_1(pin_deriction_1), pin_deriction_2(pin_deriction_2){
    pin_motor = pin_motor_;
    attachInterrupt(numb_interapt, zero_controller, CHANGE);
    pinMode(pin_zero, INPUT);
    pinMode(pin_motor, OUTPUT);
    pinMode(pin_deriction_1, OUTPUT);
    pinMode(pin_deriction_2, OUTPUT);
    motor_stop();
    regulator.setDirection(NORMAL);  // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
    regulator.setLimits(100, 9500);  // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
    Timer2.enableISR();
  }

  void change_deriction(){
    bool static flag_der = false;
    flag_der = !flag_der;
    digitalWrite(pin_deriction_1, flag_der);
    digitalWrite(pin_deriction_2, !flag_der);
  }

  void motor_work(int speed_motor) {
    regulator.setpoint = speed_motor * 138;
    regulator.input = get_speed();
    timmer_alpha = 9500 - regulator.getResultTimer();
  }

  void motor_stop(){
    digitalWrite(pin_deriction_1, LOW);
    digitalWrite(pin_deriction_2, LOW);
  }

private:
  //GyverPID regulator(0.51, 3.38, 0.02, 100);
  //GyverPID regulator(0.5, 0.9, 0.008, 100);
  GyverPID regulator(0.15, 0.1, 0.004, 100);
  uint8_t pin_zero;
  uint8_t pin_deriction_1;
  uint8_t pin_deriction_2;
}
