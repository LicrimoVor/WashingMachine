#include <Arduino.h>
#include "washing.h"
#include "water.h"
#include "temperature.h"
#include "motor.h"
#include "door.h"


struct Fasade{
    // iterations - кол-во стирка-полоскание
    // time_wash = (all_time - 10 - 10*i) / i 
    // i - половина от количества итераций 
    // temperature, speed_press, time_wash (минуты), iterrations (должно быть строго четным!)
    int params_list[][] = {
        {40, 600, 20, 2}, // быстрая стирка
        {20, 250, 30, 2}, // обувь
        {30, 400, 20, 4}, // деликатная (ручная)
        {40, 500, 20, 2}, // шерсть
        {40, 600, 20, 6}, // синтетика
        {35, 350, 15, 4}, // крупногабарит
        {40, 600, 20, 6}, // спорт (+ доп полоскание)
        {75, 750, 20, 6}, // хлопок
        {75, 750, 30, 4}, // эко (+ доп полоскание)
        {50, 250, 10, 2}, // интенсивная
        // замачивание, предварительная стирка,
        // кипячение, стирка, полоскание, отжим,
        // повторение цикла, дополнительное полоскание
    };

    Fasade(){
        setup_motor();
    }

    void main(){
        door.check_door();

        if (working){
            if (door_close){


                
            
            }
            else{
                Serial.print("Закройте дверь");
            }
        }
    }

    void start_wash(uint8_t mode_){
        mode = mode_;
        wash = Washing(params_list[mode_][1], params_list[mode_][2]*60*1000, params_list[mode_][3], motor, temperature, water);
        working = true;
    }

    void setup_water(uint8_t pin_valve_1, uint8_t pin_valve_2, uint8_t pin_pump, uint8_t pin_lvl_water) {
        water(pin_valve_1, pin_valve_2, pin_pump, pin_lvl_water);
    }

    void setup_temperature(uint8_t pin_tena, uint8_t pin_temp) {
        temperature(pin_tena, pin_temp);
    }

    void setup_door(uint8_t pin_chek, uint8_t pin_setup) {
        door(pin_chek, pin_setup);
    }

    void setup_motor(uint8_t pin_motor_, uint8_t pin_zero, uint8_t pin_deriction_1, uint8_t pin_deriction_2, uint8_t numb_interapt){
        motor(pin_motor_, pin_zero, pin_deriction_1, pin_deriction_2, numb_interapt);
    }


private:
    Water water;
    Temprature temperature;
    Door door;
    Motor motor;
    Washing wash;
    uint8_t mode = 0;
    bool working = false;
    bool door_close;
    bool water_lvl;
};
