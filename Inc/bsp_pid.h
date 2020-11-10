#ifndef __BSP_PID_H__
#define __BSP_PID_H__

#include "pid.h"
#include "bsp_can.h"

extern int16_t Motor_Speed[7];

void PID_Bsp_Init(void);

#endif
