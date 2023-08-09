#define PIN_MOTOR 4
#define PIN_ZERO 2
#define PIN_DERICTION_1 A2
#define PIN_DERICTION_2 A3

#include <Arduino.h>
#include <GyverPID.h>
#include <GyverTimers.h>
#include <GyverMotor.h>

#include "tachometer.h"
#include "door.h"

//GyverPID regulator(0.51, 3.38, 0.02, 100);
//GyverPID regulator(0.5, 0.9, 0.008, 100);
GyverPID regulator(0.15, 0.1, 0.004, 100);
uint32_t timmer_alpha;


void setup_motor() {
  attachInterrupt(0, zero_controller, CHANGE);
  pinMode(PIN_ZERO, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_DERICTION_1, OUTPUT);
  pinMode(PIN_DERICTION_2, OUTPUT);
  digitalWrite(PIN_DERICTION_1, LOW);
  digitalWrite(PIN_DERICTION_2, LOW);
  regulator.setDirection(NORMAL);  // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(100, 9500);  // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  Timer2.enableISR();
}


void zero_controller() {
  static int lastDim;
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


void motor_work(int speed_motor) {
  regulator.setpoint = speed_motor * 138;
  regulator.input = get_speed();
  timmer_alpha = 9500 - regulator.getResultTimer();
}


void change_deriction(bool flag_der){
  digitalWrite(PIN_DERICTION_1, flag_der);
  digitalWrite(PIN_DERICTION_2, !flag_der);
}


void motor_stop(){
  digitalWrite(PIN_DERICTION_1, LOW);
  digitalWrite(PIN_DERICTION_2, LOW);
}