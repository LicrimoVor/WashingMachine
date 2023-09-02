#include <Arduino.h>
#include <Tachometer.h>

Tachometer tacho;

void isr2() {
  tacho.tick();   // сообщаем библиотеке об этом
}

void setup_tachometer(uint8_t pin_tacho){
  attachInterrupt(digitalPinToInterrupt(pin_tacho), isr2, FALLING);
  pinMode(pin_tacho, INPUT);
  tacho.setWindow(50);
}

uint32_t get_speed() {
  return tacho.getRPM();
}
