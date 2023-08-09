#define PIN_VALVE_1 5
#define PIN_VALVE_2 6
#define PIN_PUMP 7
#define PIN_LVL_WATER A1

#define PIN_TENA 8
#define PIN_TEMP A4

#define TACH_PIN 3    // пин тахометра 

#include <Arduino.h>
#include "water.h"
#include "temperature.h"
#include "tachometer.h"
#include "motor.h"
#include "door.h"

Water water(PIN_VALVE_1, PIN_VALVE_2, PIN_PUMP, PIN_LVL_WATER);
Temprature temperature(PIN_TENA, PIN_TEMP);

uint32_t speed_motor__;

void setup() {
  Serial.begin(9600);
  setup_tachometer(TACH_PIN);
}

void loop() {
  static uint32_t tmr;
  motor_work(speed_motor__);
  parsing();
  if (millis() - tmr > 100) {
    tmr = millis();
    Serial.print(speed_motor__);
    Serial.print(" ");
    Serial.print(get_speed() / 138);
    Serial.print(" ");
    Serial.print(temperature.get_temp());
    Serial.print(" ");
    Serial.print(check_door());
    Serial.print(" ");
    Serial.println(water.check_water());
  }
}

void parsing() {
  if (Serial.available() > 1) {
    
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
//      case 'p': regulator.Kp = value; break;
//      case 'i': regulator.Ki = value; break;
//      case 'd': regulator.Kd = value; break;
      case 's': speed_motor__=value; break;
      case 'z': change_deriction(value); break;
      case 'e': motor_stop(); break;
      case 't': temperature.on_tena(value); break;
      case 'q': water.set_valve_1(value); break;
      case 'a': water.set_valve_2(value); break;
      case 'w': water.set_pump(value); break;
      case 'c': close_door(); break;
      case 'o': open_door(); break;
    }
  }
}
