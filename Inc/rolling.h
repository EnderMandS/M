#ifndef __ROLLING_H__
#define __ROLLING_H__

#include "main.h"
#include "max7219.h"

#define Interrupt_Display_Flag 1

#define Number_Offset 1
#define Big_Letter_Offset Number_Offset+10
#define Small_Letter_Offset Big_Letter_Offset+26

#define Words_BUF_LEN 200

typedef struct{
	uint8_t Rolling_Data[8][LED_Number];
	uint8_t Rolling_cnt;
	uint8_t Rolling_point;
	uint16_t Roll_Num;
	uint8_t ReDisplay_cnt;
	uint16_t Len;
	uint8_t String[Words_BUF_LEN];
	uint8_t String_BUF[Words_BUF_LEN];
}Rolling_Struct;
#define ReDisplay LED_Number+2

extern Rolling_Struct Rolling;

void Rolling_Init(void);
void Rolling_Byte(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Words_To_String(uint8_t *dst,const uint8_t *sourse);
void Interrupt_Display(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
