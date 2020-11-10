#ifndef __BSP_USART_H__
#define __BSP_USART_H__

#include "main.h"
#include <stdio.h>
#include <stdarg.h>

#define ZHENG 0x03

extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_uart7_tx;
extern int16_t Tr_Data[];

void sw_send_data(uint8_t *data,uint8_t len);
void SW_Send_Motor_Data(uint8_t Motor_ID);

#endif
