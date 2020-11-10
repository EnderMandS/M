#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include "main.h"

void CAN_cmd_chassis1(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_chassis2(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_chassis3(int16_t motor1, int16_t motor2, int16_t motor3);
void can_filter_init(void);
	
#endif
