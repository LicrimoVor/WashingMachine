#include <Arduino.h>
#include "washing.cpp"
#include "water.cpp"
#include "temperature.cpp"
#include "motor.cpp"
#include "door.cpp"


struct Fasade {
public:
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

  void main() {
    if (door.check_door() && working) {
      if (wash.main_wash()) {
        working = false;
      }
    } else {
      if (wash.stop_wash()) {
        door.open_door();
      }
    }
  }

  void start_wash(uint8_t mode_) {
    mode = mode_;
    Washing _wash(
      params_list[mode_][0], params_list[mode_][1], params_list[mode_][2] * 60 * 1000,
      params_list[mode_][2], motor, temperature, water);
    wash = _wash;
    working = true;
  }

  void stop_wash() {
    working = false;
  }

  void setup_water(uint8_t pin_valve_1, uint8_t pin_valve_2, uint8_t pin_pump, uint8_t pin_lvl_water) {
    Water _water(pin_valve_1, pin_valve_2, pin_pump, pin_lvl_water);
    water = _water;
  }

  void setup_temperature(uint8_t pin_tena, uint8_t pin_temp) {
    Temprature _temperature(pin_tena, pin_temp);
    temperature = _temperature;
  }

  void setup_door(uint8_t pin_chek, uint8_t pin_setup) {
    Door _door(pin_chek, pin_setup);
    door = _door;
  }

  void setup_motor(uint8_t pin_motor_, uint8_t pin_zero, uint8_t pin_deriction_1, uint8_t pin_deriction_2, uint8_t numb_interapt) {
    Motor _motor(pin_motor_, pin_zero, pin_deriction_1, pin_deriction_2, numb_interapt);
    motor = _motor;
  }


private:
  Water water;
  Temprature temperature;
  Door door;
  Motor motor;
  Washing wash;
  uint8_t mode = 0;
  bool working = false;
  bool door_close;
  bool water_lvl;
};
