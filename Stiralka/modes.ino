struct Stirka {
  public:
    // time_wash - продолжительность стирки в мс
    // time_press - продолжительность отжима в мс
    // speed_press - скорость отжима в мс
    // temperature - температура стирки
    uint32_t time_wash;
    uint32_t time_press;
    uint8_t speed_press;
    uint8_t temperature;

    void main_wash() {
      if (flag_runing) {
      switch (flag_status) {
          case 0:
            washing();
            break;
          case 1:
//            pressing();
            break;
          case 2:

            break;
        }
      }
    }


  private:
    // flag_runing - если 1 запущено, 0 не запущено
    // flag_status:
    // 0 - стирка
    // 1 - отжим
    // 2 - end
    // 3 - еще че нибудь (глажка, замачивание и тд.)
    // timmer_work - отсчет режмов работы
    // direction - направление вращения
    bool flag_runing;
    int flag_status;
    uint32_t timmer_work = 0;
    bool direction_;

    void washing() {
      uint32_t static time_10_sec = 0;
      if (timmer_work == 0) {
        timmer_work = millis();
      }

      if (millis() < (timmer_work + time_wash)) {
        motor_work();
      }
    }
};
