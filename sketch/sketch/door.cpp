#line 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\door.cpp"
#include <Arduino.h>


struct Door{
  Door(uint8_t pin_chek, uint8_t pin_setup):
    pin_chek(pin_chek), pin_setup(pin_setup) {
      pinMode(pin_setup, OUTPUT);
      pinMode(pin_chek, INPUT);
      digitalWrite(pin_setup, HIGH);
    }

  void close_door(){
    digitalWrite(pin_setup, LOW);
  }

  void open_door() {
    digitalWrite(pin_setup, HIGH);
  }

  bool check_door() {
    uint32_t static time = 0;
    if (analogRead(pin_chek) > 500){
      time = millis();
    }
    if (millis() - time > 1000) {
      return false;
    }
    return true;
  }

private:
  uint8_t pin_chek;
  uint8_t pin_setup;
};
