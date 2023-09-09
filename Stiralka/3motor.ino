#define PIN_MOTOR 4
#define PIN_ZERO 2
#define PIN_DERICTION_1 A2
#define PIN_DERICTION_2 A3

#include <GyverPID.h>
#include <GyverTimers.h>
#include <GyverMotor.h>


namespace Motor {
uint32_t timmer_alpha;
//GyverPID regulator(0.51, 3.38, 0.02, 100);
//GyverPID regulator(0.5, 0.9, 0.008, 100);
//GyverPID regulator(0.15, 0.1, 0.004, 100);
GyverPID regulator(0.15, 0.1, 0.004, 100);
uint8_t speed_motor = 0;

void zero_controller() {
  static int lastDim;
  digitalWrite(PIN_MOTOR, 0);  // выключаем симистор
  // если значение изменилось, устанавливаем новый период
  // если нет, то просто перезапускаем со старым
  if (lastDim != timmer_alpha) Timer2.setPeriod(lastDim = timmer_alpha);
  else Timer2.restart();
}

ISR(TIMER2_A) {
  digitalWrite(PIN_MOTOR, 1);  // включаем симистор
  Timer2.stop();               // останавливаем таймер
}

void motor_stop() {
  digitalWrite(PIN_DERICTION_1, LOW);
  digitalWrite(PIN_DERICTION_2, LOW);
  // Serial.println("MOTOR_STOP");
}

void setup_motor() {
  attachInterrupt(0, zero_controller, CHANGE);
  pinMode(PIN_ZERO, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_DERICTION_1, OUTPUT);
  pinMode(PIN_DERICTION_2, OUTPUT);
  motor_stop();
  regulator.setDirection(NORMAL);  // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(100, 9500);  // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  Timer2.enableISR();
  // Serial.println("SETUP_MOTOR");
}

void change_deriction() {
  bool static flag_der = false;
  if (flag_der) {
    digitalWrite(PIN_DERICTION_1, LOW);
    digitalWrite(PIN_DERICTION_2, HIGH);
  }
  else {
    digitalWrite(PIN_DERICTION_1, HIGH);
    digitalWrite(PIN_DERICTION_2, LOW);
  }
  flag_der = !flag_der;
  // regulator = GyverPID(0.15, 0.1, 0.004, 100);
  Serial.println("change dir");
}

void motor_work(uint8_t _speed_motor) {
  if (speed_motor < _speed_motor) {
    speed_motor += 1;
  }
  else if (speed_motor > _speed_motor){
    speed_motor -= 1;
  }
  regulator.setpoint = speed_motor * 138;
  regulator.input = Tacho::get_speed();
  timmer_alpha = 9500 - regulator.getResultTimer();
}
}

void test_change_deriction(){
  Motor::change_deriction();
}


void test_motor_work(){
  Serial.println("test_motor_work");
  uint32_t time = millis();
  while (millis() - time < 3000) {
    Motor::motor_work(100);
  }
  Motor::motor_work(0);
  Motor::motor_stop();
  Serial.println("end_test");
}
