#define PIN_TENA 8
#define PIN_TEMP A4


namespace Temperature {
uint8_t temperature = 0;

void setup_temprature() {
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_TENA, OUTPUT);
  digitalWrite(PIN_TENA, LOW);
  // Serial.println("SETUP_TEMPERATURE");
}

float get_temp() {
  return (analogRead(PIN_TEMP) - 246.46) / 7.7032;
}

void set_temp(uint8_t temp) {
  temperature = temp;
}

void on_tena(bool off_on) {
  digitalWrite(PIN_TENA, off_on);
}

void support_temp() {
  if (get_temp() < temperature) {
    on_tena(1);
  } else {
    on_tena(0);
  }
}
}
