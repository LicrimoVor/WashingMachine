uint32_t speed_motor;

void setup() {
  Serial.begin(9600);
  setup_tachometer();
  setup_motor();
  setup_temp();
  setup_door();
  setup_water();
}

void loop() {
  static uint32_t tmr;
  motor_work();
  parsing();
  if (millis() - tmr > 100) {
    tmr = millis();
    Serial.print(speed_motor);
    Serial.print(" ");
    Serial.print(get_speed() / 138);
    Serial.print(" ");
    Serial.print(get_temp());
    Serial.print(" ");
    Serial.print(check_door());
    Serial.print(" ");
    Serial.println(check_water());
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
      case 's': speed_motor=value; break;
      case 'z': change_deriction(value); break;
      case 't': on_tena(value); break;
      case 'q': set_valve_1(value); break;
      case 'a': set_valve_2(value); break;
      case 'w': set_pump(value); break;
      case 'e': stop_motor(); break;
      case 'c': close_door(); break;
      case 'o': open_door(); break;
    }
  }
}
