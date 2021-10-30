/*
 * led.cpp
 *
 *  Created on: Aug 1, 2021
 *      Author: huang
 */

#include <led/Led.hpp>

namespace MyDrivers {

Led::Led(int number) : number { number }
{
}

void Led::switch_led(bool_switch led_switch)
{
    switch(number)
    {
        case static_cast<int>(led_number::number1):
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, static_cast<GPIO_PinState>(led_switch));
            break;
        case static_cast<int>(led_number::number2):
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, static_cast<GPIO_PinState>(led_switch));
            break;
        default:
            break;
    }
}

void Led::on(void)
{
    switch_led(OFF);
}

void Led::off(void)
{
    switch_led(ON);
}

void Led::toggle(void)
{
    switch(number)
    {
        case static_cast<int>(led_number::number1):
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            break;
        case static_cast<int>(led_number::number2):
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
            break;
        default:
            break;
    }
}

}
