#include <Arduino.h>

struct Temprature
{
  Temprature(uint8_t pin_tena, uint8_t pin_temp):
  pin_tena(pin_tena), pin_temp(pin_temp){
    pinMode(pin_temp, INPUT);
    pinMode(pin_tena, OUTPUT);
    digitalWrite(pin_tena, LOW);
  }

  float get_temp() {
    return (analogRead(pin_temp)-246.46)/7.7032;
  }

  void on_tena(bool off_on){
    digitalWrite(pin_tena, off_on);
  }

private:
  uint8_t pin_tena;
  uint8_t pin_temp;
};
