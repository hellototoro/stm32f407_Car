/*
 * led.h
 *
 *  Created on: Aug 1, 2021
 *      Author: huang
 */

#ifndef MYDRIVERS_LED_LED_H_
#define MYDRIVERS_LED_LED_H_

#include "main.h"

namespace MyDrivers {

class Led {
public:
    Led(int number);
    void on(void);
    void off(void);
    void toggle(void);
private:
    int number;
    enum led_number {
        led1 = 1,
        led2
    };

private:
    void switch_led(bool_switch led_switch);
};
}

#endif /* MYDRIVERS_LED_LED_H_ */
