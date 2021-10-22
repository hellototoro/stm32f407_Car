/*
 * delay.h
 *
 *  Created on: Sep 20, 2021
 *      Author: huang
 */

#ifndef COMMON_INC_DELAY_H_
#define COMMON_INC_DELAY_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void delay_start(void);
extern void delay_us(uint32_t us);
extern void delay_interrupt_callback(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_INC_DELAY_H_ */
