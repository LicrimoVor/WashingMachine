#define TACH_PIN 3    // пин тахометра 

#include <Tachometer.h>
Tachometer tacho;


void setup_tachometer(){
  attachInterrupt(1, isr, FALLING);
  pinMode(TACH_PIN, INPUT);
  tacho.setWindow(50);
}


void isr() {
  tacho.tick();   // сообщаем библиотеке об этом
}


uint32_t get_speed() {
  return tacho.getRPM();
}
