

bool flag_start = false;
void setup() {
  Serial.begin(19200);
  setup_all();
}

void loop() {
  parsing();
  uint32_t static __time = millis();
  if (flag_start && millis() - __time > 100) {
    main_wash();
    __time = millis();
  }
}


void parsing() {
  if (Serial.available() > 1) {
    char incoming = Serial.read();
    int value = Serial.parseInt();
    switch (incoming) {
      case 's':
        Serial.println("start...");
        flag_start = true;
        start_wash(value);
        break;
      case 'b': stop_wash(); break;
      case 'z':
        Serial.println("out water...");
        water_out(value);
        break;
    }
  }
};
