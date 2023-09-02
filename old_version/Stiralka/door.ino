#define PIN_CHECK_LOCK_DOOR A0
#define PIN_SETUP_LOCK_DOOR 9


void setup_door() {
  pinMode(PIN_SETUP_LOCK_DOOR, OUTPUT);
  pinMode(PIN_CHECK_LOCK_DOOR, INPUT);
  digitalWrite(PIN_SETUP_LOCK_DOOR, HIGH);
}


void close_door(){
  digitalWrite(PIN_SETUP_LOCK_DOOR, LOW);
}


void open_door() {
  digitalWrite(PIN_SETUP_LOCK_DOOR, HIGH);
}

int check_door() {
  return analogRead(PIN_CHECK_LOCK_DOOR);
}
