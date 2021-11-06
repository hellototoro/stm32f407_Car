/*
 * Key.cpp
 *
 *  Created on: Nov 4, 2021
 *      Author: huang
 */

#include <key/Key.hpp>
#include "cmsis_os.h"

namespace MyDrivers {

Key::Key(int key_number) {
    // TODO Auto-generated constructor stub
    status = false;
    switch (static_cast<key_id>(key_number))
    {
    case key_id::key0:
        port = KEY0_GPIO_Port;
        pin = KEY0_Pin;
        break;

    case key_id::key1:
        port = KEY1_GPIO_Port;
        pin = KEY1_Pin;
        break;
    
    default:
        break;
    }
}

void Key::keyTask(void)
{
    if ( GPIO_PIN_RESET == HAL_GPIO_ReadPin(port, pin) ) {
        osDelay(50);
        if ( GPIO_PIN_RESET == HAL_GPIO_ReadPin(port, pin) ) {
            status = true;
            while(GPIO_PIN_RESET == HAL_GPIO_ReadPin(port, pin)) {
                osDelay(100);
            }
        }
    }
}

bool Key::keyIsPress(void)
{
    if (status) {
        status = false;
        return true;
    }
    return false;
}

Key::~Key() {
    // TODO Auto-generated destructor stub
}

} /* namespace MyDrivers */
