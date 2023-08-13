#include <Arduino.h>
#include "water.h"
#include "motor.h"
#include "temperature.h"


struct Washing {
public:
  // time_wash - продолжительность стирки в мс
  uint32_t time_wash = 600000;
  // time_rinsing - продолжительность полоскания в мс
  uint32_t time_rinsing = 600000;
  // time_press - продолжительность отжима в мс
  uint32_t time_press = 600000;
  // speed_wash - скорость стирки в об/мин
  uint8_t speed_wash = 60;
  // speed_press - скорость отжима в об/мин
  uint8_t speed_press;
  // temperature_wash - температура стирки C
  uint8_t temperature_wash;
  

  Washing(uint8_t temperature_wash, uint8_t speed_press, uint32_t time_wash, uint8_t iterations, Motor motor, Temprature temperature, Water water):
    temperature_wash(temperature_wash), speed_press(speed_press), time_wash(time_wash),
    iterations(iterations), motor(motor), temperature(temperature), water(water){
      time_start = millis();
      if (water.check_water()) {
        status = 7;
      }
      else {
        status = 1;
      }
    }

  void main_wash() {
    temperature.support_temp();
    
    water.check_water();

    temperature.set_temp(temperature);
  }


private:
  // iterations - количество итераций стирка/полоскание
  uint8_t iterations;
  // время начала стирки
  uint32_t time_start;
  // timmer_work - отсчет режмов работы
  // для отслеживания времени протекания работы одного режима работы
  uint32_t timmer_work = 0;
  // номер итерации, которая выполняется
  uint8_t iter_ = 0;
  // status - статусы системы:
  // 0 - выключено, 1 - включено
  // для мотора 1 - стирка/полоскание, 2 - отжим, 3 - медленное вращение
  // помпа, клапан_1, клапан_2, тена, мотор
  uint8_t status_data[][] = {
    {0, 0, 0, 0, 0}, // все выключено
    {0, 1, 0, 0, 0}, // запуск воды для предварительной стирки/полоскание
    {0, 0, 1, 0, 0}, // запуск воды для основной стирки
    {0, 1, 1, 0, 0}, // запуск воды для кондиционера
    {0, 0, 0, 1, 1}, // стирка/полоскание
    {0, 0, 0, 1, 2}, // отжим
    {0, 0, 0, 1, 3}, // нагрев воды
    {1, 0, 0, 0, 0}, // вывод воды
  };
  uint8_t status = 0;
  Motor motor;
  Temprature temperature;
  Water water;

  void iterations() {
    if (timmer_work == 0) {
      timmer_work = millis();
    }




  }

  void __washing() {
    uint32_t static time_10_sec = 0;

  void __pump    

    if (millis() < (timmer_work + time_wash)) {
      motor_work(speed_wash);
    }
  }
}
