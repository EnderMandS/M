#ifndef __LIGHT_UP_H__
#define __LIGHT_UP_H__

#include "main.h"

typedef struct
{
	uint8_t State;									//状态 OFF ON HIT
	GPIO_TypeDef* Matrix_CS_GPIOx;	//LED点阵片选
	uint16_t Matrix_CS_GPIO_Pin;
	GPIO_TypeDef* Top_GPIOx;				//顶部灯条
	uint16_t Top_GPIO_Pin;
	GPIO_TypeDef* Beside_GPIOx;			//侧边灯条
	uint16_t Beside_Pin;
}Wing_Struct;
typedef struct
{
	uint8_t Wing_ID;	// 扇叶ID 1-5
	uint8_t cnt;	//击打计时，在定时器里加 单位0.1s
	uint8_t Start_State;	//开始计时标志位
	uint8_t Shining_cnt;	//这个值是闪烁次数的两倍
	uint8_t Hit_Success_cnt;	//击打成功计数
}Hit_Struct;

#define Use_TIM_cnt TIM1_cnt
#define OFF	0
#define ON 	1
#define HIT	2
#define Wing_Num 5

#define Wing_1_Matrix_CS_GPIOx 			GPIOA
#define Wing_1_Matrix_CS_GPIO_Pin		GPIO_PIN_6
#define Wing_1_Top_GPIOx						GPIOA
#define Wing_1_Top_GPIO_Pin					GPIO_PIN_8
#define Wing_1_Beside_GPIOx					GPIOA
#define Wing_1_Beside_Pin						GPIO_PIN_9

#define Wing_2_Matrix_CS_GPIOx 			GPIOA
#define Wing_2_Matrix_CS_GPIO_Pin		GPIO_PIN_6
#define Wing_2_Top_GPIOx						GPIOA
#define Wing_2_Top_GPIO_Pin					GPIO_PIN_8
#define Wing_2_Beside_GPIOx					GPIOA
#define Wing_2_Beside_Pin						GPIO_PIN_9

#define Wing_3_Matrix_CS_GPIOx 			GPIOA
#define Wing_3_Matrix_CS_GPIO_Pin		GPIO_PIN_6
#define Wing_3_Top_GPIOx						GPIOA
#define Wing_3_Top_GPIO_Pin					GPIO_PIN_8
#define Wing_3_Beside_GPIOx					GPIOA
#define Wing_3_Beside_Pin						GPIO_PIN_9

#define Wing_4_Matrix_CS_GPIOx 			GPIOA
#define Wing_4_Matrix_CS_GPIO_Pin		GPIO_PIN_6
#define Wing_4_Top_GPIOx						GPIOA
#define Wing_4_Top_GPIO_Pin					GPIO_PIN_8
#define Wing_4_Beside_GPIOx					GPIOA
#define Wing_4_Beside_Pin						GPIO_PIN_9

#define Wing_5_Matrix_CS_GPIOx 			GPIOA
#define Wing_5_Matrix_CS_GPIO_Pin		GPIO_PIN_6
#define Wing_5_Top_GPIOx						GPIOA
#define Wing_5_Top_GPIO_Pin					GPIO_PIN_8
#define Wing_5_Beside_GPIOx					GPIOA
#define Wing_5_Beside_Pin						GPIO_PIN_9

extern Wing_Struct Wing[Wing_Num];

uint16_t Rand_TIM(uint16_t Min, uint16_t Max);
void Change_Work_State(uint8_t Wing_ID, uint8_t State);
void Wind_Init(void);
void Wind_Test(void);
void TIM_Hit_Cnt(void);
void Judge_Hit_Check(void);

#endif
