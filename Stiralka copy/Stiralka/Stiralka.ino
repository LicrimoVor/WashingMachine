#define PIN_VALVE_1 5
#define PIN_VALVE_2 6
#define PIN_PUMP 7
#define PIN_LVL_WATER A1

#define PIN_TENA 8
#define PIN_TEMP A4

#define TACH_PIN 3

#define PIN_CHECK_LOCK_DOOR A0
#define PIN_SETUP_LOCK_DOOR 9

#define PIN_MOTOR 4
#define PIN_ZERO 2
#define PIN_DERICTION_1 A2
#define PIN_DERICTION_2 A3
#define NUMB_INTERRUPT 0

#include <Arduino.h>
// #include "tacho.ino"
#include "fasad.h"

Fasade fasade;

void setup() {
  Serial.begin(9600);
  setup_tachometer(TACH_PIN);
  fasade.setup_temperature(PIN_TENA, PIN_TEMP);
  fasade.setup_water(PIN_VALVE_1, PIN_VALVE_2, PIN_PUMP, PIN_LVL_WATER);
  fasade.setup_door(PIN_CHECK_LOCK_DOOR, PIN_SETUP_LOCK_DOOR);
  fasade.setup_motor(PIN_MOTOR, PIN_ZERO, PIN_DERICTION_1, PIN_DERICTION_2, NUMB_INTERRUPT);
}

void loop() {
  parsing();
  fasade.main();
}


void parsing() {
  if (Serial.available() > 1) {
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
      case 's': fasade.start_wash(value); break;
      case 'b': fasade.stop_wash(); break;
    }
  }
}
