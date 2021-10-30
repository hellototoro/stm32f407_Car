/*
 * ultrasonic.cpp
 *
 *  Created on: Sep 20, 2021
 *      Author: huang
 */

#include <ultrasonic/Ultrasonic.hpp>
#include "board.h"

Ultrasonic::Ultrasonic() {
    // TODO Auto-generated constructor stub

}

void Ultrasonic::trig(void)
{
    send_trig();
}

Ultrasonic::~Ultrasonic() {
    // TODO Auto-generated destructor stub
}

