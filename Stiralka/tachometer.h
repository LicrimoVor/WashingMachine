#include <Arduino.h>
#include <Tachometer.h>

Tachometer tacho;

void setup_tachometer(uint8_t pin_tacho){
  attachInterrupt(1, isr, FALLING);
  pinMode(pin_tacho, INPUT);
  tacho.setWindow(50);
}

void isr() {
  tacho.tick();   // сообщаем библиотеке об этом
}

uint32_t get_speed() {
  return tacho.getRPM();
}
