#include <Arduino.h>

struct Water{
public:
  Water(uint8_t pin_valve_1, uint8_t pin_valve_2, uint8_t pin_pump, uint8_t pin_lvl_water):
    pin_valve_1(pin_valve_1), pin_valve_2(pin_valve_2), pin_pump(pin_pump), pin_lvl_water(pin_lvl_water)
  {
    pinMode(pin_valve_1, OUTPUT);
    pinMode(pin_valve_2, OUTPUT);
    pinMode(pin_pump, OUTPUT);
    pinMode(pin_lvl_water, INPUT);
    digitalWrite(pin_valve_1, HIGH);
    digitalWrite(pin_valve_2, HIGH);
    digitalWrite(pin_pump, LOW);
  }

  void set_valve_1(bool open_close){
  // предварительная стирка
  digitalWrite(pin_valve_1, open_close);
  }

  void set_valve_2(bool open_close){
  digitalWrite(pin_valve_2, open_close);
  }

  void set_pump(bool open_close) {
  digitalWrite(pin_pump, open_close);
  }

  int check_water() {
  return analogRead(pin_lvl_water);
  }

private:
  uint8_t pin_valve_1;
  uint8_t pin_valve_2;
  uint8_t pin_pump;
  uint8_t pin_lvl_water;
};



