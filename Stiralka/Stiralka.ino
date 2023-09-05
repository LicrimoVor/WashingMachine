bool flag_start = false;
bool flag_test = false;

void setup() {
  Serial.begin(19200);
  setup_all();
}

void loop() {
  parsing();
  uint32_t static __time = millis();
  if (millis() - __time > 100) {
    if (flag_start ) {main_wash();}
    // Serial.println(__time);
    __time = millis();
  }
  if (flag_test) {
    test_motor_work();
  }
}


void parsing() {
  if (Serial.available() > 1) {
    char incoming = Serial.read();
    int value = Serial.parseFloat();
    switch (incoming) {
      case 's':
        Serial.println("start...");
        flag_start = true;
        start_wash(value);
        break;
      case 'b': stop_wash(); break;
      case 'z':
        Serial.println("out water...");
        test_water_out(value);
        break;
      case 'v':
        test_speed();
        break;
      case 'd':
        test_door(value);
        break;
      case 'w':
        flag_test = value;
        break;
      case 'c':
        test_change_deriction();
        break;
    }
  }
};
