#include <Arduino.h>


void setup() {
  Serial.begin(9600);
  setup_all();
}

void loop() {
  parsing();
  main_wash();
}


void parsing() {
  if (Serial.available() > 1) {
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
      case 's': start_wash(value); break;
      case 'b': stop_wash(); break;
    }
  }
}
