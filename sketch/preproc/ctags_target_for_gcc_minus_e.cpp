# 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino"
# 2 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\Stiralka.ino" 2


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
# 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\1door.ino"



# 5 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\1door.ino" 2

namespace Door {
void close_door() {
  digitalWrite(9, 0x0);
}

void setup_door() {
  pinMode(9, 0x1);
  pinMode(A0, 0x0);
  digitalWrite(9, 0x1);
}

void open_door() {
  digitalWrite(9, 0x1);
}

bool check_door() {
  uint32_t static time = 0;
  if (analogRead(A0) > 500) {
    time = millis();
  }
  if (millis() - time > 1000) {
    return false;
  }
  return true;
}
}
# 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\2tacho.ino"


# 4 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\2tacho.ino" 2


namespace Tacho {
Tachometer tacho;

void isr() {
  tacho.tick(); // сообщаем библиотеке об этом
}

uint32_t get_speed() {
  return tacho.getRPM();
}

void setup_tachometer() {
  attachInterrupt(1, isr, 2);
  pinMode(3, 0x0);
  tacho.setWindow(50);
}
}
# 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\3motor.ino"





# 7 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\3motor.ino" 2
# 8 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\3motor.ino" 2
# 9 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\3motor.ino" 2
# 10 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\3motor.ino" 2


namespace Motor {
uint32_t timmer_alpha;
//GyverPID regulator(0.51, 3.38, 0.02, 100);
//GyverPID regulator(0.5, 0.9, 0.008, 100);
//GyverPID regulator(0.15, 0.1, 0.004, 100);
GyverPID regulator(0.15, 0.1, 0.004, 100);

void zero_controller() {
  static int lastDim;
  digitalWrite(4, 0); // выключаем симистор
  // если значение изменилось, устанавливаем новый период
  // если нет, то просто перезапускаем со старым
  if (lastDim != timmer_alpha) Timer2.setPeriod(lastDim = timmer_alpha);
  else Timer2.restart();
}


# 28 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\3motor.ino" 3
extern "C" void __vector_12 /* Timer/Counter1 Compare Match B */ (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_12 /* Timer/Counter1 Compare Match B */ (void) 
# 28 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\3motor.ino"
             {
  digitalWrite(4, 1); // включаем симистор
  Timer2.stop(); // останавливаем таймер
}

void motor_stop() {
  digitalWrite(A2, 0x0);
  digitalWrite(A3, 0x0);
}

void setup_motor() {
  attachInterrupt(((2) == 2 ? 0 : ((2) == 3 ? 1 : -1)), zero_controller, 1);
  pinMode(2, 0x0);
  pinMode(4, 0x1);
  pinMode(A2, 0x1);
  pinMode(A3, 0x1);
  motor_stop();
  regulator.setDirection(0); // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(100, 9500); // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  Timer2.enableISR();
}

void change_deriction() {
  bool static flag_der = false;
  flag_der = !flag_der;
  digitalWrite(A2, flag_der);
  digitalWrite(A3, !flag_der);
}

void motor_work(int speed_motor) {
  regulator.setpoint = speed_motor * 138;
  regulator.input = Tacho::get_speed();
  timmer_alpha = 9500 - regulator.getResultTimer();
}
}
# 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\4temperature.ino"



# 5 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\4temperature.ino" 2


namespace Temperature {
uint8_t temperature = 0;

void setup_temprature() {
  pinMode(A4, 0x0);
  pinMode(8, 0x1);
  digitalWrite(8, 0x0);
}

float get_temp() {
  return (analogRead(A4) - 246.46) / 7.7032;
}

void set_temp(uint8_t temp) {
  temperature = temp;
}

void on_tena(bool off_on) {
  digitalWrite(8, off_on);
}

void support_temp() {
  if (get_temp() < temperature) {
    on_tena(1);
  } else {
    on_tena(0);
  }
}
}
# 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\5water.ino"





# 7 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\5water.ino" 2


namespace Water {
void setup_water() {
  pinMode(5, 0x1);
  pinMode(6, 0x1);
  pinMode(7, 0x1);
  pinMode(A1, 0x0);
  digitalWrite(5, 0x1);
  digitalWrite(6, 0x1);
  digitalWrite(7, 0x0);
}

void set_wash(bool valve_1, bool valve_2) {
  digitalWrite(5, valve_1);
  digitalWrite(6, valve_2);
}

void set_pump(bool open_close) {
  digitalWrite(7, open_close);
}

bool check_water() {
  uint32_t static time = 0;
  if (analogRead(A1) > 500) {
    time = millis();
  }
  if (millis() - time > 1000) {
    return false;
  }
  return true;
}
}
# 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\6washing.ino"
# 2 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\6washing.ino" 2


namespace Washing {
// time_wash - продолжительность итерации стирки в мс (15 минут)
uint32_t time_wash = 900000;
// time_rinsing - продолжительность полоскания в мс (10 минут)
uint32_t time_rinsing = 600000;
// time_press - продолжительность отжима в мс (10 минут)
uint32_t time_press = 600000;
// speed_wash - скорость стирки в об/мин
uint8_t speed_wash = 60;
// speed_press - скорость отжима в об/мин
uint8_t speed_press;
// temperature_wash - температура стирки C
uint16_t temperature_wash;
// iterations - количество итераций стирка/полоскание
uint16_t iterations;
// timmer_work - отсчет режмов работы
// для отслеживания времени протекания работы одного режима работы
uint32_t timmer_work = 0;
// номер итерации, которая выполняется
uint8_t iter_ = 0;
// flag_water = True если надо работать с водой
// flag_pump_valve - False - помпа, True - клапаны
bool flag_water, flag_pump_valve;
bool valve_1, valve_2;

void create_washing(uint16_t _temperature_wash, uint16_t _speed_press, uint32_t _time_wash, uint16_t _iterations) {
  temperature_wash = _temperature_wash;
  speed_press = _speed_press;
  time_wash = _time_wash;
  iterations = _iterations;
  flag_water = true;
  flag_pump_valve = true;
  valve_1 = true;
  valve_2 = false;
  timmer_work = millis() + time_wash;
}

bool stop_wash() {
  Temperature::support_temp();
  Temperature::set_temp(0);
  Motor::motor_work(0);
  Motor::motor_stop();
  if (Water::check_water()) {
    Water::set_pump(true);
  } else {
    Water::set_pump(false);
    if (Temperature::get_temp() < 40) {
      return true;
    }
  }
  return false;
}

void variable_wash(uint8_t speed, uint32_t duration) {
  uint32_t static time_10_sec = millis() + duration;
  // 0 - не крутится, 1 - крутится
  bool static flag = true;

  if ((millis() < time_10_sec) && (flag)) {
    Motor::motor_work(speed_wash);
  } else {
    if (millis() > time_10_sec) {
      Motor::motor_work(0);
      Motor::motor_stop();
      if (millis() > time_10_sec + 10000) {
        time_10_sec = millis() + duration;
        flag = !flag;
        if (flag) { Motor::change_deriction(); }
      }
    }
  }
}

void iter_wash() {
  bool static flag_0 = true;
  if (millis() < timmer_work && flag_0) {
    variable_wash(speed_wash, 10000);
  } else {
    // время полоскания 600000 (10 минут)
    bool static flag_1 = true;
    if (flag_1) {
      Motor::motor_work(0);
      Motor::motor_stop();
      flag_water = true;
      flag_pump_valve = false;
      flag_1 = false;
      flag_0 = false;
      timmer_work = millis() + time_rinsing / 2;
    } else {
      bool static flag_2 = true;
      if (flag_2) {
        if (millis() < timmer_work) {
          variable_wash(speed_press, 30000);
        } else {
          switch (iterations) {
            case 1:
              valve_1 = true;
              valve_2 = true;
              break;
            case 2:
              valve_1 = false;
              valve_2 = true;
              break;
            case 3:
              valve_1 = true;
              valve_2 = false;
              break;
          }
          timmer_work = millis() + time_rinsing / 2;
          flag_2 = false;
        }
      } else {
        if (millis() < timmer_work + time_rinsing / 2) {
          variable_wash(200, 60000);
        } else {
          flag_0 = true;
          flag_1 = true;
          flag_2 = true;
          iterations -= 1;
          timmer_work = millis() + time_wash;
        }
      }
    }
  }
}



void water_refresh() {
  if (!flag_pump_valve) {
    if (Water::check_water()) {
      Water::set_pump(true);
    } else {
      Water::set_pump(false);
      flag_water = false;
    }
  } else {
    if (!Water::check_water()) {
      Water::set_wash(valve_1, valve_2);
    } else {
      Water::set_wash(false, false);
      flag_water = false;
    }
  }
}

bool end_wash() {
  bool static flag = true;
  if (flag) {
    Motor::motor_work(0);
    Motor::motor_stop();
    flag_water = true;
    flag_pump_valve = false;
    timmer_work = millis() + time_press;
  } else {
    if (millis() < timmer_work) {
      variable_wash(speed_press, 30000);
    } else {
      return true;
    }
  }
  return false;
}


bool main_wash() {
  if (flag_water) {
    water_refresh();
  } else {
    if (iterations > 0) {
      if (Water::check_water()) {
        Temperature::support_temp();
        Temperature::set_temp(temperature_wash);
      } else {
        Temperature::set_temp(0);
      }
      iter_wash();
    } else {
      return end_wash();
    }
  }
  return false;
}

};
# 1 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\7fasad.ino"
# 2 "C:\\Disk D\\Programm's\\Arduino\\stiralka\\Stiralka\\7fasad.ino" 2


// namespace Fasad {
// iterations - кол-во стирка+полоскание
// time_wash = (all_time - 10 - 10*i) / i
// temperature, speed_press, time_wash (минуты), iterrations (должно быть строго четным!)
uint16_t params_list[10][4]{
  { 40, 600, 20, 1 }, // быстрая стирка
  { 20, 250, 30, 1 }, // обувь
  { 30, 400, 20, 2 }, // деликатная (ручная)
  { 40, 500, 20, 1 }, // шерсть
  { 40, 600, 20, 3 }, // синтетика
  { 35, 350, 15, 2 }, // крупногабарит
  { 40, 600, 20, 3 }, // спорт (+ доп полоскание)
  { 75, 750, 20, 3 }, // хлопок
  { 75, 750, 30, 2 }, // эко (+ доп полоскание)
  { 50, 250, 10, 1 }, // интенсивная
                       // замачивание, предварительная стирка,
                       // кипячение, стирка, полоскание, отжим,
                       // повторение цикла, дополнительное полоскание
};
bool working = false;
bool door_close;
bool water_lvl;

void start_wash(uint8_t mode) {
  Washing::create_washing(
    params_list[mode][0], params_list[mode][1],
    params_list[mode][2] * 60 * 1000, params_list[mode][2]);
  working = true;
}

void stop_wash() {
  working = false;
}
// };

void main_wash() {
  if (Door::check_door() && working) {
    if (Washing::main_wash()) {
      working = false;
    }
  } else {
    if (Washing::stop_wash()) {
      Door::open_door();
    }
  }
}

void setup_all(){
  Temperature::setup_temprature();
  Water::setup_water();
  Door::setup_door();
  Motor::setup_motor();
  Tacho::setup_tachometer();
}
