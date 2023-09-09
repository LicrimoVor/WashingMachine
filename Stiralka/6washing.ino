

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
  time_wash = _time_wash * 60 * 1000;
  iterations = _iterations;
  flag_water = true;
  flag_pump_valve = true;
  valve_1 = true;
  valve_2 = false;
  timmer_work = millis() + time_wash;
  Serial.print("Start: ");
  Serial.println(time_wash);
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
  uint32_t static time_duraction = millis();

  if (millis() < time_duraction) {
    Motor::motor_work(speed);
  } else {
    if (millis() > time_duraction) {
      Motor::motor_work(0);
      // Motor::motor_stop();
      if (millis() > time_duraction + 6000) {
        time_duraction = millis() + duration;
        Motor::change_deriction();
      }
    }
  }
}

void iter_wash() {
  bool static flag_0 = true;
  bool static flag_print = true;
  if (millis() < timmer_work && flag_0) {
    variable_wash(speed_wash, 10000);
    if (flag_print) {
      Serial.println("motor_run");
      flag_print = false;
    }
  } else {
    // время полоскания 600000 (10 минут)
    bool static flag_1 = true;
    flag_print = true;
    if (flag_1) {
      Motor::motor_work(0);
      Motor::motor_stop();
      flag_water = true;
      flag_pump_valve = false;
      flag_1 = false;
      flag_0 = false;
      timmer_work = millis() + time_rinsing / 2;
      Serial.println("ITER_OUT_WATER");
    } else {
      bool static flag_2 = true;
      if (flag_2) {
        if (millis() < timmer_work) {
          variable_wash(speed_press, 30000);
          if (flag_print) {
            Serial.println("motor_run");
            flag_print = false;
          }
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
          flag_print = true;
          Serial.println("ITER_2");
        }
      } else {
        if (millis() < timmer_work + time_rinsing / 2) {
          variable_wash(200, 60000);
          if (flag_print) {
            Serial.println("motor_run");
            flag_print = false;
          }
        } else {
          flag_0 = true;
          flag_1 = true;
          flag_2 = true;
          flag_print = true;
          iterations -= 1;
          timmer_work = millis() + time_wash;
          Serial.println("ITER_3");
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
      Serial.println("Pump_off");
    }
  } else {
    if (!Water::check_water()) {
      Water::set_wash(valve_1, valve_2);
    } else {
      Water::set_wash(false, false);
      flag_water = false;
      Serial.println("Water_off");
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
      // Serial.println("iter_end");
    } else {
      return end_wash();
    }
  }
  return false;
}
};


void test_varible_speed(int value) {
  Serial.println("test_varible_speed");
  uint32_t time = millis();
  bool flag = true;
  while (millis() - time < 1800000 && flag) {
    Washing::variable_wash(value, 60000);
    if (Serial.available() > 1) {flag = false;}
  }
  Motor::motor_work(0);
  Motor::motor_stop();
  Serial.println("end_test");
}
