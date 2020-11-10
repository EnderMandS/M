#ifndef __BSP_DMA_H__
#define __BSP_DMA_H__

#include "main.h"

HAL_StatusTypeDef Bsp_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef Bsp_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
void Dma_UsartIdleHanlder(UART_HandleTypeDef *huart,uint16_t Size);

extern uint8_t Judgement_Buf[];
extern uint8_t Rolling_Words_BUF[];

#endif
