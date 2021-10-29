/**
  ******************************************************************************
  * @file    motor_control.c
  * @brief   
  * @data    2021-7-28
  * @author  jee
  ******************************************************************************
  */
#include "main.h"
#include "motor_control.h"


#define WHEEL_DIAMETER  0.048f      //轮子直径，单位：m
#define PI              3.1415f     //Π
#define WHEEL_CPR       1080.f      //每转计数
#define ENCODE_RESOLUTION   ((PI*WHEEL_DIAMETER)/WHEEL_CPR)

#define PWM_RESOLUTION  255.f


/*  */
#define TWO_WHEEL
#ifdef  TWO_WHEEL
#define MOTOR_PORT(dir,polarity)    dir##_MOTOR_##polarity##_GPIO_Port  //dir: @ LEFT RIGHT FRONT BACK
                                                                        //polarity: @ P N
#define MOTOR_PIN(dir,polarity)     dir##_MOTOR_##polarity##_Pin        //dir: @ LEFT RIGHT FRONT BACK
                                                                        //polarity: @ P N

#define PWM_HANDLE(dir)         dir##_motor_pwm                         //dir: @ LEFT RIGHT FRONT BACK
#define PWM_Start(dir) \
  (((&(PWM_HANDLE(dir))) == &(PWM_HANDLE(LEFT))) ? (HAL_TIM_PWM_Start(PWM_HANDLE(dir), TIM_CHANNEL_1)) :\
   (HAL_TIM_PWM_Start(PWM_HANDLE(dir), TIM_CHANNEL_2)))                 //dir: @ LEFT RIGHT FRONT BACK

#define SET_DutyCycle(dir,d) \
  (((&(PWM_HANDLE(dir))) == &(PWM_HANDLE(LEFT))) ? (Configure_DutyCycle(PWM_HANDLE(dir), TIM_CHANNEL_1, d)) :\
   (Configure_DutyCycle(PWM_HANDLE(dir), TIM_CHANNEL_2, d)))            //dir: @ LEFT RIGHT FRONT BACK
                                                                        //d:占空比
#else
#define MOTOR_PORT(dir,polarity)    dir##_motor_##polarity##_GPIO_Port  //left_front_motor_A_GPIO_Port
#define MOTOR_PIN(dir,polarity)     dir##_motor_##polarity##_Pin  //left_front_motor_A_Pin

#define PWM_HANDLE(dir)         dir##_motor_pwm
#define STR1(R)  #R
#define STR2(R)  STR1(R)  
#define PWM_Start(dir) \
  (((&(PWM_HANDLE(dir))) == &(PWM_HANDLE(left_front))) ? (HAL_TIM_PWM_Start(PWM_HANDLE(dir), TIM_CHANNEL_1)) :\
  (((&(PWM_HANDLE(dir))) == &(PWM_HANDLE(right_front))) ? (HAL_TIM_PWM_Start(PWM_HANDLE(dir), TIM_CHANNEL_2)) :\
  (((&(PWM_HANDLE(dir))) == &(PWM_HANDLE(left_back))) ? (HAL_TIM_PWM_Start(PWM_HANDLE(dir), TIM_CHANNEL_1)) :\
   (HAL_TIM_PWM_Start(PWM_HANDLE(dir), TIM_CHANNEL_2)))))

#define SET_DutyCycle(dir,d) \
  (((&(PWM_HANDLE(dir))) == &(PWM_HANDLE(left_front))) ? (Configure_DutyCycle(PWM_HANDLE(dir), TIM_CHANNEL_1, d)) :\
  (((&(PWM_HANDLE(dir))) == &(PWM_HANDLE(right_front))) ? (Configure_DutyCycle(PWM_HANDLE(dir), TIM_CHANNEL_2, d)) :\
  (((&(PWM_HANDLE(dir))) == &(PWM_HANDLE(left_back))) ? (Configure_DutyCycle(PWM_HANDLE(dir), TIM_CHANNEL_1, d)) :\
   (Configure_DutyCycle(PWM_HANDLE(dir), TIM_CHANNEL_2, d)))))
#endif

//#define MILLISECOND_TO_SECOND(millisecond)  


//static uint8_t pwmStart_flag = 0;


motor_info motor;

/*      静态函数声明     */
//static void calculate_speed(uint16_t time, uint32_t encoder_counter, double *speed);
static void calculate_rpm(uint16_t time, int32_t delta_counter, double *rpm);
static void Configure_DutyCycle(PWM_HandleTypeDef *PWM_Handle, uint32_t Channel, uint16_t D);


/**
  * @brief  Changes the duty cycle of the PWM signal.
  *         D = (T/P)*PWM_RESOLUTION
  *           where T is the pulse duration and P is the PWM signal period
  * @param  D Duty cycle
  *         from 0 to 255
  * @retval None
  */
static void Configure_DutyCycle(PWM_HandleTypeDef *PWM_Handle, uint32_t Channel, uint16_t D)
{
    uint32_t P;    /* Pulse duration */
    uint32_t T;    /* PWM signal period */

    /* PWM signal period is determined by the value of the auto-reload register */
    T = __HAL_TIM_GET_AUTORELOAD(PWM_Handle) + 1;

    /* Pulse duration is determined by the value of the compare register.       */
    /* Its value is calculated in order to match the requested duty cycle.      */
    P = (D*T)/PWM_RESOLUTION;
    __HAL_TIM_SET_COMPARE(PWM_Handle, Channel, P);
}

void Motor_PID_Input(Motor_WheelType wheel, uint16_t input)
{
    (wheel == wheel_left) ? SET_DutyCycle(LEFT,input) : SET_DutyCycle(RIGHT,input);
}

void Motor_start(void)
{
    PWM_Start(LEFT);
    PWM_Start(RIGHT);
}

void Motor_init(void)
{
    Motor_start();
    SET_DutyCycle(LEFT,200);
    SET_DutyCycle(RIGHT,200);
}


void Motor_moveFRONT(void)
{
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,P), MOTOR_PIN(LEFT,P), GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,N), MOTOR_PIN(LEFT,N), GPIO_PIN_RESET);
    
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,P), MOTOR_PIN(RIGHT,P), GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,N), MOTOR_PIN(RIGHT,N), GPIO_PIN_RESET);
}

void Motor_moveRIGHT(void)
{
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,P), MOTOR_PIN(LEFT,P), GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,N), MOTOR_PIN(LEFT,N), GPIO_PIN_RESET);
    
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,P), MOTOR_PIN(RIGHT,P), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,N), MOTOR_PIN(RIGHT,N), GPIO_PIN_SET);
}

void Motor_moveBACK(void)
{
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,P), MOTOR_PIN(LEFT,P), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,N), MOTOR_PIN(LEFT,N), GPIO_PIN_SET);
    
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,P), MOTOR_PIN(RIGHT,P), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,N), MOTOR_PIN(RIGHT,N), GPIO_PIN_SET);
}

void Motor_moveLEFT(void)
{
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,P), MOTOR_PIN(LEFT,P), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,N), MOTOR_PIN(LEFT,N), GPIO_PIN_SET);
    
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,P), MOTOR_PIN(RIGHT,P), GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,N), MOTOR_PIN(RIGHT,N), GPIO_PIN_RESET);
}

void Motor_moveSTOP(void)
{
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,P), MOTOR_PIN(LEFT,P), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,N), MOTOR_PIN(LEFT,N), GPIO_PIN_RESET);
    
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,P), MOTOR_PIN(RIGHT,P), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,N), MOTOR_PIN(RIGHT,N), GPIO_PIN_RESET);
}

void Motor_lock(void)
{
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,P), MOTOR_PIN(LEFT,P), GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT(LEFT,N), MOTOR_PIN(LEFT,N), GPIO_PIN_SET);
    
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,P), MOTOR_PIN(RIGHT,P), GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT(RIGHT,N), MOTOR_PIN(RIGHT,N), GPIO_PIN_SET);
}

/*******************************编码器相关****************************/
//time单位：ms
//static void calculate_speed(uint16_t time, uint32_t encoder_counter, double *speed)
//{
    //#define RESOLUTION  ((PI*WHEEL_DIAMETER)/WHEEL_CPR)//分辨率
    //double distance = RESOLUTION*encoder_counter;
    //*speed = (distance/time)*1000;

//}

//time单位：ms
//rpm单位：转/秒
static void calculate_rpm(uint16_t time, int32_t delta_counter, double *rpm)
{
    *rpm = (delta_counter * 1000 * 60.f) / (WHEEL_CPR * (double)time);//转/分钟
    //*rpm = (delta_counter * 1000.f) / (WHEEL_CPR * (double)time);
}


/*  每隔 @ TIM7_TICK_FREQ_DEFAULT 周期调用一次 */
void Motor_encoderTask(void)
{
    //getEncoderInfo(LEFT_encoder,&motor.left_encoder);
    //getEncoderInfo(RIGHT_encoder,&motor.right_encoder);
    int32_t counter_l = __HAL_TIM_GET_COUNTER(LEFT_encoder) + 
                        (motor.left_encoder.overflow_count * __HAL_TIM_GET_AUTORELOAD(LEFT_encoder));
    int32_t counter_r = __HAL_TIM_GET_COUNTER(RIGHT_encoder) + 
                        (motor.right_encoder.overflow_count * __HAL_TIM_GET_AUTORELOAD(RIGHT_encoder));

    motor.left_encoder.delta_counter = counter_l - motor.left_encoder.last_counter;
    motor.right_encoder.delta_counter = counter_r - motor.right_encoder.last_counter;

    calculate_rpm(TIM6_TICK_FREQ_DEFAULT, motor.left_encoder.delta_counter, &motor.left_wheel.current_rpm);
    calculate_rpm(TIM6_TICK_FREQ_DEFAULT, motor.right_encoder.delta_counter, &motor.right_wheel.current_rpm);
    motor.left_wheel.mileage += motor.left_encoder.delta_counter * ENCODE_RESOLUTION;   /* 累计里程 */
    motor.right_wheel.mileage += motor.right_encoder.delta_counter * ENCODE_RESOLUTION; /* 累计里程 */

    motor.left_encoder.last_counter = counter_l;
    motor.right_encoder.last_counter = counter_r;
}

//速度单位：m/s
double getSpeed(Motor_WheelType wheel)
{
    double rpm = (wheel == wheel_left) ? motor.left_wheel.current_rpm : motor.right_wheel.current_rpm;
    return rpm * (PI * WHEEL_DIAMETER);
}

double getSpeed_RPM(Motor_WheelType wheel)
{
	return (wheel == wheel_left) ? motor.left_wheel.current_rpm : motor.right_wheel.current_rpm;
}

double getMileage(Motor_WheelType wheel)
{
	return (wheel == wheel_left) ? motor.left_wheel.mileage : motor.right_wheel.mileage;
}


