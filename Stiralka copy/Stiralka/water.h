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

  void set_wash(bool valve_1, bool valve_2){
    digitalWrite(pin_valve_1, valve_1);
    digitalWrite(pin_valve_2, valve_2);
  }

  void set_pump(bool open_close) {
    digitalWrite(pin_pump, open_close);
  }

  bool check_water() {
    uint32_t static time = 0;
    if (analogRead(pin_lvl_water) > 500){
      time = millis();
    }
    if (millis() - time > 1000) {
      return false;
    }
    return true;
  }

private:
  uint8_t pin_valve_1;
  uint8_t pin_valve_2;
  uint8_t pin_pump;
  uint8_t pin_lvl_water;
};
