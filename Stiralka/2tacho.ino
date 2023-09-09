#define PIN_TACHO 3

#include <Tachometer.h>


namespace Tacho {
Tachometer tacho;

void isr() {
  tacho.tick();  // сообщаем библиотеке об этом
}

uint32_t get_speed() {
  // Serial.println("Get_speed");
  return tacho.getRPM();
}

void setup_tachometer() {
  attachInterrupt(1, isr, FALLING);
  pinMode(PIN_TACHO, INPUT);
  tacho.setWindow(50);
  // Serial.println("SETUP_TACHO");
}
}

