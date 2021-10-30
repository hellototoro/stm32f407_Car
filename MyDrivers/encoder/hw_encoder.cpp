/*
 * hw_encoder.cpp
 *
 *  Created on: Oct 26, 2021
 *      Author: huang
 */

#include "encoder/hw_encoder.hpp"

namespace MyDrivers {

void hw_encoder::init(Encoder_HandleTypeDef *Encoder_Handle)
{
    this->Encoder_Handle = Encoder_Handle;
}

void hw_encoder::start(void)
{
    HAL_TIM_Encoder_Start_IT(Encoder_Handle, TIM_CHANNEL_ALL);
}

void hw_encoder::stop(void)
{
    HAL_TIM_Encoder_Stop_IT(Encoder_Handle, TIM_CHANNEL_ALL);
}

void hw_encoder::interrput(hw_encoder &encoder)
{
    (__HAL_TIM_IS_TIM_COUNTING_DOWN(encoder.Encoder_Handle) == true) ? encoder.overflow-- : encoder.overflow++;
}

} /* namespace MyDrivers */
