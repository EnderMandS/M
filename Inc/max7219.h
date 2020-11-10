#ifndef __MAX7219_H__
#define __MAX7219_H__

#include "main.h"

#define LED_Number 		4
#define LED_Matrix_SPI &hspi1

#define No_Op 				0x00	//空指令
#define Digit0				0x01
#define Digit1				0x02
#define Digit2				0x03
#define Digit3				0x04
#define Digit4				0x05
#define Digit5				0x06
#define Digit6				0x07
#define Digit7				0x08
#define Decode_Mode 	0x09	//编码模式
#define Intensity			0x0A	//亮度
#define Scan_Limit		0x0B	//扫描模式
#define ShutDown			0x0C	//掉电模式 0开 1关
#define Display_Test 	0x0F	//显示测试 0关 1开

//SPI1 
//PA6 CS/LOAD
//PA5 CLk
//PA7 MOSI
#define MAX7219_LOAD_GPIOTpye 	GPIOA
#define MAX7219_LOAD_Pin 				GPIO_PIN_6

void Write_MAX7219(SPI_HandleTypeDef *hspi,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *pData, uint16_t Size);
void Write_MAX7219_Multi_Copy(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t Num, uint8_t Cmd, uint8_t Data);
void Write_MAX7219_Single(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t ID, uint8_t Cmd, uint8_t Data);
void LED_Matrix_Init(void);
void Pic_8_Display(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Light_Up_All(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void OFF_All(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t Combine_uint8_t(uint8_t Data1, uint8_t Data2);

#endif
