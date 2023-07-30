#include <Arduino.h>
#include "motor.h"
#include "tacho.h"

uint32_t speed_in_circle;

void setup() {
  Serial.begin(9600);
  setup_tachometer();
  setup_motor();
}


void loop() {
  static uint32_t tmr;
  speed_in_circle = map(analogRead(0), 0, 1023, 0, 600);
  set_motor_speed(speed_in_circle);
  if (millis() - tmr > 100) {
    tmr = millis();
    Serial.print(get_speed()/138);
    Serial.print(" ");
    Serial.print(speed_in_circle);
    Serial.print(" ");
    Serial.println(get_timmer_alpha());
  }
}
