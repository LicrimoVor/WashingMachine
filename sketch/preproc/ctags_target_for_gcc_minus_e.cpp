# 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino"
# 20 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino"
# 21 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino" 2
# 22 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino" 2
# 23 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino" 2

Fasade fasade;

void setup() {
  Serial.begin(9600);
  setup_tachometer(3);
  fasade.setup_temperature(8, A4);
  fasade.setup_water(5, 6, 7, A1);
  fasade.setup_door(A0, 9);
  fasade.setup_motor(4, 2, A2, A3, 0);
}

void loop() {
  parsing();
  fasade.main();
}


void parsing() {
  if (Serial.available() > 1) {
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
      case 's': fasade.start_wash(value); break;
      case 'b': fasade.stop_wash(); break;
    }
  }
}
