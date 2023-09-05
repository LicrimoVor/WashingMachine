#define PIN_VALVE_1 5
#define PIN_VALVE_2 6
#define PIN_PUMP 7
#define PIN_LVL A1


namespace Water {
void setup_water() {
  pinMode(PIN_VALVE_1, OUTPUT);
  pinMode(PIN_VALVE_2, OUTPUT);
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_LVL, INPUT);
  digitalWrite(PIN_VALVE_1, HIGH);
  digitalWrite(PIN_VALVE_2, HIGH);
  digitalWrite(PIN_PUMP, LOW);
  // Serial.println("SETUP_WATER");
}

void set_wash(bool valve_1, bool valve_2) {
  digitalWrite(PIN_VALVE_1, valve_1);
  digitalWrite(PIN_VALVE_2, valve_2);
  Serial.println("do water");
}

void set_pump(bool close_open) {
  digitalWrite(PIN_PUMP, close_open);
  // Serial.println("OUTPUT_WATER");
}

bool check_water() {
  uint32_t static time = 0;
  if (analogRead(PIN_LVL) > 500) {
    time = millis();
  }
  if (millis() - time > 1000) {
    return false;
  }
  return true;
}
}

void test_water_out(bool lol){
  if (lol) {Door::close_door();}
  else {Door::open_door();}
  Water::set_pump(lol);
}