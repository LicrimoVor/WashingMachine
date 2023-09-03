#define PIN_CHECK_DOOR A0
#define PIN_SETUP_DOOR 9


namespace Door {
void close_door() {
  digitalWrite(PIN_SETUP_DOOR, LOW);
  Serial.println("DOOR_CLOSE");
}

void open_door() {
  digitalWrite(PIN_SETUP_DOOR, HIGH);
  Serial.println("DOOR_OPEN");
}

void setup_door() {
  pinMode(PIN_SETUP_DOOR, OUTPUT);
  pinMode(PIN_CHECK_DOOR, INPUT);
  open_door();
  // Serial.println("SETUP_DOOR");
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
