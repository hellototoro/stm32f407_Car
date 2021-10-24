#include <encoder/encoder.h>

#define INIT_COUNTER    (32768-1)

void startEncoder(Encoder_HandleTypeDef **Encoder_Handle, uint8_t count)
{
    HAL_TIM_Base_Start_IT(encoder_timer);
    for (uint8_t i = 0; i < count; i++) {
        //__HAL_TIM_SET_COUNTER(Encoder_Handle[i], INIT_COUNTER);
        /* 使能定时器的更新事件中断 */
        __HAL_TIM_ENABLE_IT(Encoder_Handle[i],TIM_IT_UPDATE);
        /* 设置更新事件请求源为：计数器溢出 */
        __HAL_TIM_URS_ENABLE(Encoder_Handle[i]);
        HAL_TIM_Encoder_Start(Encoder_Handle[i], TIM_CHANNEL_ALL);
    }
}

void stopEncoder(Encoder_HandleTypeDef **Encoder_Handle,uint8_t count)
{
    for (uint8_t i = 0; i < count; i++) {
        HAL_TIM_Encoder_Stop(Encoder_Handle[i], TIM_CHANNEL_ALL);
    }
}

/* 此函数必须周期性调用，以便获取单位时间内的编码增量，推荐每10ms调用一次 */
void getEncoderInfo(Encoder_HandleTypeDef *Encoder_Handle,encoder_info *encoder)
{
    //test_encoder = __HAL_TIM_GET_COUNTER(encoders[0]) - (32768-1);
    encoder->delta_counter = __HAL_TIM_GET_COUNTER(Encoder_Handle);
    //__HAL_TIM_SET_COUNTER(Encoder_Handle, INIT_COUNTER);
}

void updateEncoderCounter(Encoder_HandleTypeDef *Encoder_Handle, encoder_info *encoder)
{
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(Encoder_Handle))
        encoder->overflow_count--;
    else
        encoder->overflow_count++;
}

