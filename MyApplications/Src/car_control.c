/**
  ******************************************************************************
  * @file    car_control.c
  * @brief   
  * @data    2021-7-28
  * @author  jee
  ******************************************************************************
  */
#include "car_control.h"

#include "motor_control.h"


Car_control Car;

void Car_init(void)
{
    Car.v_dir = 0;
    Car.h_dir = 0;
    Car.speed = 0;
    Motor_init();
}

uint8_t Car_isLastData(Car_control *data)
{
    if ( data->h_dir == Car.h_dir && 
         data->v_dir == Car.v_dir && 
         data->speed == Car.speed )
        return 1;
    else
        return 0;
}

void Car_monitor(void)
{
    static Car_control last_dir = {0};
    if (!Car_isLastData(&last_dir)) {
        last_dir = Car;
        if (Car.v_dir*Car.h_dir == 0)
        {
            if (Car.v_dir == 1) {
                Motor_move(FRONT);
            } else if (Car.v_dir == -1) {
                Motor_move(BACK);
            } else if (Car.h_dir == 1) {
                Motor_move(RIGHT);
            } else if (Car.h_dir == -1) {
                Motor_move(LEFT);
            } else {
                Motor_lock();
            }
        } else {
            Motor_lock();
        }
    }
}
