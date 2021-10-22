/*
 * delay.c
 *
 *  Created on: Sep 20, 2021
 *      Author: huang
 */
#include "delay.h"

static __IO uint32_t wait;
static __IO uint32_t tickstart;

void delay_start(void)
{
    HAL_TIM_Base_Start_IT(usDelay_timer_Handle);
}

void delay_us(__IO uint32_t us)
{
	__HAL_TIM_SET_COUNTER(usDelay_timer_Handle,0);
    wait = us;

    /* Add a freq to guarantee minimum wait */
    if (wait < HAL_MAX_DELAY)
    {
        wait += (uint32_t)(uwTickFreq);
    }

    while((__HAL_TIM_GET_COUNTER(usDelay_timer_Handle)) < wait)
    {
    }
}

void delay_interrupt_callback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == usDelay_timer_Handle->Instance) {
    	__HAL_TIM_SET_COUNTER(usDelay_timer_Handle,0);
        wait = wait - 65536;
    }
}
