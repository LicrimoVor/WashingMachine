uint32_t speed_motor;

void setup() {
  Serial.begin(9600);
  setup_tachometer();
  setup_motor();
}

void loop() {
  static uint32_t tmr;
  motor_work();
  parsing();
  if (millis() - tmr > 100) {
    tmr = millis();
    Serial.print(speed_motor);
    Serial.print(" ");
    Serial.print(get_timmer_alpha() / 100);
    Serial.print(" ");
    Serial.println(get_speed() / 138);
  }
}
