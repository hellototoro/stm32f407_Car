/*
 * hw_encoder.cpp
 *
 *  Created on: Oct 26, 2021
 *      Author: huang
 */

#include <encoder/HW_Encoder.hpp>

namespace MyDrivers {

void HW_Encoder::init(Encoder_HandleTypeDef *Encoder_Handle)
{
    this->Encoder_Handle = Encoder_Handle;
}

void HW_Encoder::start(void)
{
    __HAL_TIM_SET_COUNTER(Encoder_Handle, 0);
    __HAL_TIM_CLEAR_IT(Encoder_Handle, TIM_IT_UPDATE);
    /* 使能定时器的更新事件中断 */
    __HAL_TIM_ENABLE_IT(Encoder_Handle,TIM_IT_UPDATE);
    /* 设置更新事件请求源为：计数器溢出 */
    __HAL_TIM_URS_ENABLE(Encoder_Handle);
    HAL_TIM_Encoder_Start(Encoder_Handle, TIM_CHANNEL_ALL);
    //HAL_TIM_Encoder_Start_IT(Encoder_Handle, TIM_CHANNEL_ALL);
}

void HW_Encoder::stop(void)
{
    //HAL_TIM_Encoder_Stop_IT(Encoder_Handle, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Stop(Encoder_Handle, TIM_CHANNEL_ALL);
}

void HW_Encoder::interrput(HW_Encoder &encoder)
{
    (__HAL_TIM_IS_TIM_COUNTING_DOWN(encoder.Encoder_Handle) == true) ? encoder.overflow-- : encoder.overflow++;
}

} /* namespace MyDrivers */
