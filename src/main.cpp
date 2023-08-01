#define PIN_MOTOR 4
#define PIN_ZERO 2
#define TACH_PIN 3    // пин тахометра 

#include <Arduino.h>
#include "motor.h"

uint32_t speed_in_circle;
Motor motor(PIN_MOTOR, PIN_ZERO, TACH_PIN);

void isr_tacho(){
  motor._isr_tacho();
}

void isr_zero(){
  motor._isr_zero();
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(1, isr_tacho, FALLING);
  attachInterrupt(0, isr_zero, CHANGE);
}

void loop() {
  static uint32_t tmr;
  speed_in_circle = map(analogRead(0), 0, 1023, 0, 600);
  motor.set_speed(speed_in_circle);
  if (millis() - tmr > 100) {
    tmr = millis();
    Serial.print(motor.get_speed()/138);
    Serial.print(" ");
    Serial.print(speed_in_circle);
    Serial.print(" ");
    Serial.println(motor.timmer_alpha);
  }
}
