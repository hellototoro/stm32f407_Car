/*
 * encoder.hpp
 *
 *  Created on: Oct 26, 2021
 *      Author: huang
 */

#ifndef ENCODER_HW_ENCODER_HPP_
#define ENCODER_HW_ENCODER_HPP_

#include "main.h"

//#define getEncoderCounter(Encoder_Handle)       __HAL_TIM_GET_COUNTER(Encoder_Handle)
//#define getEncoderDirection(Encoder_Handle)     __HAL_TIM_IS_TIM_COUNTING_DOWN(Encoder_Handle)

namespace MyDrivers {

class HW_Encoder {
public:
    __IO int32_t delta_counter;     /* 编码器单位时间内的增量 */
    __IO int32_t last_counter;
    __IO int32_t sum_counter;
    __IO int16_t overflow;    /* 定时器溢出次数 */

private:
    constexpr static double line = 3.f;
    constexpr static double reduction_ratio = 85.f;
    Encoder_HandleTypeDef *Encoder_Handle;

public:
    HW_Encoder() {}
    constexpr static double resolution(uint8_t encoder_mode) {
        return line * reduction_ratio * encoder_mode;
    }
    void init(Encoder_HandleTypeDef *Encoder_Handle);
    void start(void);
    void stop(void);
    uint32_t getEncoderCounter(void) {
        return __HAL_TIM_GET_COUNTER(Encoder_Handle);
    }
    uint32_t getPeriod(void) {
        return (__HAL_TIM_GET_AUTORELOAD(Encoder_Handle) + 1);
    }
    static void interrput(HW_Encoder &encoder);

    virtual ~HW_Encoder() {}
};

} /* namespace MyDrivers */

#endif /* ENCODER_HW_ENCODER_HPP_ */
