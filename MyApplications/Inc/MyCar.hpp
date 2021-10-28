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

class MyCar {
public:
    MyCar();
    void init(void);
    void power_on(void);
    void power_off(void);
    void move_front(double speed, double distance = 0);
    void move_back(double speed, double distance = 0);
    void turn_left(double speed, double angle = 0);
    void turn_right(double speed, double angle = 0);
    void stop(void);
    virtual ~MyCar();
public:
    enum direction{
        front,
        back,
        left,
        right
    };
    MyWheel left_wheel;
    MyWheel right_wheel;
private:


private:
    void Configure_DutyCycle(PWM_HandleTypeDef *PWM_Handle, uint32_t Channel, uint16_t D);
};

#endif /* INC_MYCAR_HPP_ */
