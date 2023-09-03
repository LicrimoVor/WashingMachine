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
  Door::close_door();
  working = true;
  Serial.println("START_WASH");
}

void stop_wash() {
  working = false;
  Serial.println("STOP_WASH");
}
// };

void main_wash() {
  bool static flag_print = true;
  if (Door::check_door() && working) {
    if (!flag_print) {
      flag_print = true;
    }
    if (Washing::main_wash()) {
      working = false;
    }
  } else {
    if (working) {
      if (flag_print) {
        Serial.println("close_door_plz");
        flag_print = false;
      }

    } else {
      if (Washing::stop_wash()) {
        Door::open_door();
        Serial.println("END_WASH");
        if (!flag_print) {
          flag_print = true;
        }
      }
    }
  }
}

void setup_all() {
  Tacho::setup_tachometer();
  Motor::setup_motor();
  Temperature::setup_temprature();
  Water::setup_water();
  Door::setup_door();
  Serial.println("end_setup");
}
