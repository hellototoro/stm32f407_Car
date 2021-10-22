/*
 * ultrasonic.cpp
 *
 *  Created on: Sep 20, 2021
 *      Author: huang
 */

#include "ultrasonic/ultrasonic.hpp"

#include "board.h"

ultrasonic::ultrasonic() {
    // TODO Auto-generated constructor stub

}

void ultrasonic::trig(void)
{
    send_trig();
}

ultrasonic::~ultrasonic() {
    // TODO Auto-generated destructor stub
}

