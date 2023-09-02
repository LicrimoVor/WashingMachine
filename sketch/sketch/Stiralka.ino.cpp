#line 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino"
#include <Arduino.h>


#line 4 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino"
void setup();
#line 9 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino"
void loop();
#line 15 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino"
void parsing();
#line 27 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\7fasad.ino"
void start_wash(uint8_t mode);
#line 34 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\7fasad.ino"
void stop_wash();
#line 39 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\7fasad.ino"
void main_wash();
#line 51 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\7fasad.ino"
void setup_all();
#line 4 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino"
void setup() {
  Serial.begin(9600);
  setup_all();
}

void loop() {
  parsing();
  main_wash();
}


void parsing() {
  if (Serial.available() > 1) {
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
      case 's': start_wash(value); break;
      case 'b': stop_wash(); break;
    }
  }
}

#line 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\1door.ino"
#define PIN_CHECK_DOOR A0
#define PIN_SETUP_DOOR 9

#include <Arduino.h>

namespace Door {
void close_door() {
  digitalWrite(PIN_SETUP_DOOR, LOW);
}

void setup_door() {
  pinMode(PIN_SETUP_DOOR, OUTPUT);
  pinMode(PIN_CHECK_DOOR, INPUT);
  digitalWrite(PIN_SETUP_DOOR, HIGH);
}

void open_door() {
  digitalWrite(PIN_SETUP_DOOR, HIGH);
}

bool check_door() {
  uint32_t static time = 0;
  if (analogRead(PIN_CHECK_DOOR) > 500) {
    time = millis();
  }
  if (millis() - time > 1000) {
    return false;
  }
  return true;
}
}

#line 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\2tacho.ino"
#define PIN_TACHO 3

#include <Tachometer.h>


namespace Tacho {
Tachometer tacho;

void isr() {
  tacho.tick();  // сообщаем библиотеке об этом
}

uint32_t get_speed() {
  return tacho.getRPM();
}

void setup_tachometer() {
  attachInterrupt(1, isr, FALLING);
  pinMode(PIN_TACHO, INPUT);
  tacho.setWindow(50);
}
}


#line 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\3motor.ino"
#define PIN_MOTOR 4
#define PIN_ZERO 2
#define PIN_DERICTION_1 A2
#define PIN_DERICTION_2 A3

#include <Arduino.h>
#include <GyverPID.h>
#include <GyverTimers.h>
#include <GyverMotor.h>


namespace Motor {
uint32_t timmer_alpha;
//GyverPID regulator(0.51, 3.38, 0.02, 100);
//GyverPID regulator(0.5, 0.9, 0.008, 100);
//GyverPID regulator(0.15, 0.1, 0.004, 100);
GyverPID regulator(0.15, 0.1, 0.004, 100);

void zero_controller() {
  static int lastDim;
  digitalWrite(PIN_MOTOR, 0);  // выключаем симистор
  // если значение изменилось, устанавливаем новый период
  // если нет, то просто перезапускаем со старым
  if (lastDim != timmer_alpha) Timer2.setPeriod(lastDim = timmer_alpha);
  else Timer2.restart();
}

ISR(TIMER1_B) {
  digitalWrite(PIN_MOTOR, 1);  // включаем симистор
  Timer2.stop();               // останавливаем таймер
}

void motor_stop() {
  digitalWrite(PIN_DERICTION_1, LOW);
  digitalWrite(PIN_DERICTION_2, LOW);
}

void setup_motor() {
  attachInterrupt(digitalPinToInterrupt(PIN_ZERO), zero_controller, CHANGE);
  pinMode(PIN_ZERO, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_DERICTION_1, OUTPUT);
  pinMode(PIN_DERICTION_2, OUTPUT);
  motor_stop();
  regulator.setDirection(NORMAL);  // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(100, 9500);  // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  Timer2.enableISR();
}

void change_deriction() {
  bool static flag_der = false;
  flag_der = !flag_der;
  digitalWrite(PIN_DERICTION_1, flag_der);
  digitalWrite(PIN_DERICTION_2, !flag_der);
}

void motor_work(int speed_motor) {
  regulator.setpoint = speed_motor * 138;
  regulator.input = Tacho::get_speed();
  timmer_alpha = 9500 - regulator.getResultTimer();
}
}
#line 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\4temperature.ino"
#define PIN_TENA 8
#define PIN_TEMP A4

#include <Arduino.h>


namespace Temperature {
uint8_t temperature = 0;

void setup_temprature() {
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_TENA, OUTPUT);
  digitalWrite(PIN_TENA, LOW);
}

float get_temp() {
  return (analogRead(PIN_TEMP) - 246.46) / 7.7032;
}

void set_temp(uint8_t temp) {
  temperature = temp;
}

void on_tena(bool off_on) {
  digitalWrite(PIN_TENA, off_on);
}

void support_temp() {
  if (get_temp() < temperature) {
    on_tena(1);
  } else {
    on_tena(0);
  }
}
}

#line 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\5water.ino"
#define PIN_VALVE_1 5
#define PIN_VALVE_2 6
#define PIN_PUMP 7
#define PIN_LVL A1

#include <Arduino.h>


namespace Water {
void setup_water() {
  pinMode(PIN_VALVE_1, OUTPUT);
  pinMode(PIN_VALVE_2, OUTPUT);
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_LVL, INPUT);
  digitalWrite(PIN_VALVE_1, HIGH);
  digitalWrite(PIN_VALVE_2, HIGH);
  digitalWrite(PIN_PUMP, LOW);
}

void set_wash(bool valve_1, bool valve_2) {
  digitalWrite(PIN_VALVE_1, valve_1);
  digitalWrite(PIN_VALVE_2, valve_2);
}

void set_pump(bool open_close) {
  digitalWrite(PIN_PUMP, open_close);
}

bool check_water() {
  uint32_t static time = 0;
  if (analogRead(PIN_LVL) > 500) {
    time = millis();
  }
  if (millis() - time > 1000) {
    return false;
  }
  return true;
}
}
#line 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\6washing.ino"
#include <Arduino.h>


namespace Washing {
// time_wash - продолжительность итерации стирки в мс (15 минут)
uint32_t time_wash = 900000;
// time_rinsing - продолжительность полоскания в мс (10 минут)
uint32_t time_rinsing = 600000;
// time_press - продолжительность отжима в мс (10 минут)
uint32_t time_press = 600000;
// speed_wash - скорость стирки в об/мин
uint8_t speed_wash = 60;
// speed_press - скорость отжима в об/мин
uint8_t speed_press;
// temperature_wash - температура стирки C
uint16_t temperature_wash;
// iterations - количество итераций стирка/полоскание
uint16_t iterations;
// timmer_work - отсчет режмов работы
// для отслеживания времени протекания работы одного режима работы
uint32_t timmer_work = 0;
// номер итерации, которая выполняется
uint8_t iter_ = 0;
// flag_water = True если надо работать с водой
// flag_pump_valve - False - помпа, True - клапаны
bool flag_water, flag_pump_valve;
bool valve_1, valve_2;

void create_washing(uint16_t _temperature_wash, uint16_t _speed_press, uint32_t _time_wash, uint16_t _iterations) {
  temperature_wash = _temperature_wash;
  speed_press = _speed_press;
  time_wash = _time_wash;
  iterations = _iterations;
  flag_water = true;
  flag_pump_valve = true;
  valve_1 = true;
  valve_2 = false;
  timmer_work = millis() + time_wash;
}

bool stop_wash() {
  Temperature::support_temp();
  Temperature::set_temp(0);
  Motor::motor_work(0);
  Motor::motor_stop();
  if (Water::check_water()) {
    Water::set_pump(true);
  } else {
    Water::set_pump(false);
    if (Temperature::get_temp() < 40) {
      return true;
    }
  }
  return false;
}

void variable_wash(uint8_t speed, uint32_t duration) {
  uint32_t static time_10_sec = millis() + duration;
  // 0 - не крутится, 1 - крутится
  bool static flag = true;

  if ((millis() < time_10_sec) && (flag)) {
    Motor::motor_work(speed_wash);
  } else {
    if (millis() > time_10_sec) {
      Motor::motor_work(0);
      Motor::motor_stop();
      if (millis() > time_10_sec + 10000) {
        time_10_sec = millis() + duration;
        flag = !flag;
        if (flag) { Motor::change_deriction(); }
      }
    }
  }
}

void iter_wash() {
  bool static flag_0 = true;
  if (millis() < timmer_work && flag_0) {
    variable_wash(speed_wash, 10000);
  } else {
    // время полоскания 600000 (10 минут)
    bool static flag_1 = true;
    if (flag_1) {
      Motor::motor_work(0);
      Motor::motor_stop();
      flag_water = true;
      flag_pump_valve = false;
      flag_1 = false;
      flag_0 = false;
      timmer_work = millis() + time_rinsing / 2;
    } else {
      bool static flag_2 = true;
      if (flag_2) {
        if (millis() < timmer_work) {
          variable_wash(speed_press, 30000);
        } else {
          switch (iterations) {
            case 1:
              valve_1 = true;
              valve_2 = true;
              break;
            case 2:
              valve_1 = false;
              valve_2 = true;
              break;
            case 3:
              valve_1 = true;
              valve_2 = false;
              break;
          }
          timmer_work = millis() + time_rinsing / 2;
          flag_2 = false;
        }
      } else {
        if (millis() < timmer_work + time_rinsing / 2) {
          variable_wash(200, 60000);
        } else {
          flag_0 = true;
          flag_1 = true;
          flag_2 = true;
          iterations -= 1;
          timmer_work = millis() + time_wash;
        }
      }
    }
  }
}



void water_refresh() {
  if (!flag_pump_valve) {
    if (Water::check_water()) {
      Water::set_pump(true);
    } else {
      Water::set_pump(false);
      flag_water = false;
    }
  } else {
    if (!Water::check_water()) {
      Water::set_wash(valve_1, valve_2);
    } else {
      Water::set_wash(false, false);
      flag_water = false;
    }
  }
}

bool end_wash() {
  bool static flag = true;
  if (flag) {
    Motor::motor_work(0);
    Motor::motor_stop();
    flag_water = true;
    flag_pump_valve = false;
    timmer_work = millis() + time_press;
  } else {
    if (millis() < timmer_work) {
      variable_wash(speed_press, 30000);
    } else {
      return true;
    }
  }
  return false;
}


bool main_wash() {
  if (flag_water) {
    water_refresh();
  } else {
    if (iterations > 0) {
      if (Water::check_water()) {
        Temperature::support_temp();
        Temperature::set_temp(temperature_wash);
      } else {
        Temperature::set_temp(0);
      }
      iter_wash();
    } else {
      return end_wash();
    }
  }
  return false;
}

};

#line 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\7fasad.ino"
#include <Arduino.h>


// namespace Fasad {
// iterations - кол-во стирка+полоскание
// time_wash = (all_time - 10 - 10*i) / i
// temperature, speed_press, time_wash (минуты), iterrations (должно быть строго четным!)
uint16_t params_list[10][4]{
  { 40, 600, 20, 1 },  // быстрая стирка
  { 20, 250, 30, 1 },  // обувь
  { 30, 400, 20, 2 },  // деликатная (ручная)
  { 40, 500, 20, 1 },  // шерсть
  { 40, 600, 20, 3 },  // синтетика
  { 35, 350, 15, 2 },  // крупногабарит
  { 40, 600, 20, 3 },  // спорт (+ доп полоскание)
  { 75, 750, 20, 3 },  // хлопок
  { 75, 750, 30, 2 },  // эко (+ доп полоскание)
  { 50, 250, 10, 1 },  // интенсивная
                       // замачивание, предварительная стирка,
                       // кипячение, стирка, полоскание, отжим,
                       // повторение цикла, дополнительное полоскание
};
bool working = false;
bool door_close;
bool water_lvl;

void start_wash(uint8_t mode) {
  Washing::create_washing(
    params_list[mode][0], params_list[mode][1],
    params_list[mode][2] * 60 * 1000, params_list[mode][2]);
  working = true;
}

void stop_wash() {
  working = false;
}
// };

void main_wash() {
  if (Door::check_door() && working) {
    if (Washing::main_wash()) {
      working = false;
    }
  } else {
    if (Washing::stop_wash()) {
      Door::open_door();
    }
  }
}

void setup_all(){
  Temperature::setup_temprature();
  Water::setup_water();
  Door::setup_door();
  Motor::setup_motor();
  Tacho::setup_tachometer();
}

