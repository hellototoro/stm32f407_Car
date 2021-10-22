#ifndef __MAIN_HPP
#define __MAIN_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern void setup(void);
extern void loop(void);
void set_target_speed(Motor_WheelType wheel, double speed);

#ifdef __cplusplus
}
#endif


#endif
