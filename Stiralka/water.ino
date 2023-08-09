#define PIN_VALVE_1 5
#define PIN_VALVE_2 6
#define PIN_PUMP 7
#define PIN_LVL_WATER A1


void setup_water() {
  pinMode(PIN_VALVE_1, OUTPUT);
  pinMode(PIN_VALVE_2, OUTPUT);
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_LVL_WATER, INPUT);
  digitalWrite(PIN_VALVE_1, 1);
  digitalWrite(PIN_VALVE_2, 1);
  digitalWrite(PIN_PUMP, LOW);
}

void set_valve_1(bool open_close){
  // предварительная стирка
  digitalWrite(PIN_VALVE_1, open_close);
}

void set_valve_2(bool open_close){
  digitalWrite(PIN_VALVE_2, open_close);
}

void set_pump(bool open_close) {
  digitalWrite(PIN_PUMP, open_close);
}

int check_water() {
  return analogRead(PIN_LVL_WATER);
}
