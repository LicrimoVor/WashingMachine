#define PIN_TENA 8
#define PIN_TEMP A4


void setup_temp() {
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_TENA, OUTPUT);
  digitalWrite(PIN_TENA, LOW);
}

float get_temp() {
  return (analogRead(PIN_TEMP)-246.46)/7.7032;
}

void on_tena(bool lol){
  digitalWrite(PIN_TENA, lol);
}
