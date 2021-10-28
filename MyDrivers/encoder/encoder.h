#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define getEncoderCounter(Encoder_Handle)       __HAL_TIM_GET_COUNTER(Encoder_Handle)
//#define getEncoderDirection(Encoder_Handle)     __HAL_TIM_IS_TIM_COUNTING_DOWN(Encoder_Handle)

extern void startEncoder(Encoder_HandleTypeDef **Encoder_Handle,uint8_t count);
extern void stopEncoder(Encoder_HandleTypeDef **Encoder_Handle,uint8_t count);

#ifdef __cplusplus
}
#endif

#endif
