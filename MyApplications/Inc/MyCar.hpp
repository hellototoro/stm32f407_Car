/*
 * MyCar.hpp
 *
 *  Created on: Oct 24, 2021
 *      Author: huang
 */

#ifndef INC_MYCAR_HPP_
#define INC_MYCAR_HPP_

#include "main.h"
#include "MyWheel.hpp"
#include <math.h>

class MyCar {
public:
    MyWheel left_wheel;
    MyWheel right_wheel;

public:
    MyCar();
    void init(void);
    void power_on(void);
    void power_off(void);
    void move_front(double speed, double distance = 0.f);
    void move_back(double speed, double distance = 0.f);
    void turn_left(double speed, double angle = 0.f);
    void turn_right(double speed, double angle = 0.f);
    void stop(void);
    void timeout_interrput(TIM_HandleTypeDef *htim);
    virtual ~MyCar();

private:
    constexpr static double radiusOfTwoWheel = 0.156f;//两轮中心距离
    constexpr static double ratio = (M_PI * radiusOfTwoWheel) / 180.f;

private:
};

#endif /* INC_MYCAR_HPP_ */
