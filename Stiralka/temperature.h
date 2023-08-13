#include <Arduino.h>


struct Temprature{
  uint8_t temperature = 0;

  Temprature(uint8_t pin_tena, uint8_t pin_temp):
    pin_tena(pin_tena), pin_temp(pin_temp){
    pinMode(pin_temp, INPUT);
    pinMode(pin_tena, OUTPUT);
    digitalWrite(pin_tena, LOW);
  }

  float get_temp() {
    return (analogRead(pin_temp)-246.46)/7.7032;
  }

  void set_temp(uint8_t temp){
    temperature = temp;
  }

  void support_temp(){
    if (get_temp() < temperature){on_tena(1);}
    else{on_tena(0);}
  }

private:
  uint8_t pin_tena;
  uint8_t pin_temp;

  void on_tena(bool off_on){
    digitalWrite(pin_tena, off_on);
  }
}
