#define PIN_CHECK_DOOR A0
#define PIN_SETUP_DOOR 9

#include <Arduino.h>

namespace Door {
void close_door() {
  digitalWrite(PIN_SETUP_DOOR, LOW);
}

void setup_door() {
  pinMode(PIN_SETUP_DOOR, OUTPUT);
  pinMode(PIN_CHECK_DOOR, INPUT);
  digitalWrite(PIN_SETUP_DOOR, HIGH);
}

void open_door() {
  digitalWrite(PIN_SETUP_DOOR, HIGH);
}

bool check_door() {
  uint32_t static time = 0;
  if (analogRead(PIN_CHECK_DOOR) > 500) {
    time = millis();
  }
  if (millis() - time > 1000) {
    return false;
  }
  return true;
}
}
