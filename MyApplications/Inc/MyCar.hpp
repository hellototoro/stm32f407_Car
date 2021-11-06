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

#define R               0.3f

class MyCar {
public:
    MyWheel left_wheel;
    MyWheel right_wheel;

public:
    MyCar();
    void init(void);
    void power_on(void);
    void power_off(void);
    void moveToSpeed(double speed_l, double speed_r);
    void moveToDistance(double distance_l, double distance_r);
    void turn(double angle, double speed = 80.f);
    void stop(void);
    void cmd_vel(double external);
    void timeout_interrput(TIM_HandleTypeDef *htim);
    virtual ~MyCar();

private:
    constexpr static double radiusOfTwoWheel = 0.055f;//两轮中心距离
    constexpr static double ratio = (M_PI * radiusOfTwoWheel) / 180.f;
    constexpr static double r_inner = R - radiusOfTwoWheel;
    constexpr static double r_external = R + radiusOfTwoWheel;

private:
    void setMoveDirection(MyWheel &wheel, double value);
};

#endif /* INC_MYCAR_HPP_ */
