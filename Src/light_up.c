#include "light_up.h"
#include "max7219.h"
#include "spi.h"
#include "judge.h"

extern uint16_t TIM1_cnt;	//用于生成随机数
Hit_Struct Hit_cnt={0}; 
Wing_Struct Wing[Wing_Num]={
	{OFF,Wing_1_Matrix_CS_GPIOx,Wing_1_Matrix_CS_GPIO_Pin,Wing_1_Top_GPIOx,Wing_1_Top_GPIO_Pin,Wing_1_Beside_GPIOx,Wing_1_Beside_Pin},
	{OFF,Wing_2_Matrix_CS_GPIOx,Wing_2_Matrix_CS_GPIO_Pin,Wing_2_Top_GPIOx,Wing_2_Top_GPIO_Pin,Wing_2_Beside_GPIOx,Wing_2_Beside_Pin},
	{OFF,Wing_3_Matrix_CS_GPIOx,Wing_3_Matrix_CS_GPIO_Pin,Wing_3_Top_GPIOx,Wing_3_Top_GPIO_Pin,Wing_3_Beside_GPIOx,Wing_3_Beside_Pin},
	{OFF,Wing_4_Matrix_CS_GPIOx,Wing_4_Matrix_CS_GPIO_Pin,Wing_4_Top_GPIOx,Wing_4_Top_GPIO_Pin,Wing_4_Beside_GPIOx,Wing_4_Beside_Pin},
	{OFF,Wing_5_Matrix_CS_GPIOx,Wing_5_Matrix_CS_GPIO_Pin,Wing_5_Top_GPIOx,Wing_5_Top_GPIO_Pin,Wing_5_Beside_GPIOx,Wing_5_Beside_Pin},
};

uint16_t Rand_TIM(uint16_t Min, uint16_t Max)	//Max必须小于等于读取定时器cnt的最大值
{
	return ( Use_TIM_cnt%(Max-Min+1) )+Min;
}
void Change_Work_State(uint8_t Wing_ID, uint8_t State)	//更改扇叶状态
{
	Wing[Wing_ID-1].State=State;
	switch (Wing[Wing_ID-1].State)
	{
		case OFF:		//全部关闭
			Hit_cnt.Start_State=OFF;
			Write_MAX7219_Multi_Copy(Wing[Wing_ID-1].Matrix_CS_GPIOx, Wing[Wing_ID-1].Matrix_CS_GPIO_Pin, LED_Number, Display_Test, 0x00);	//关闭测试模式
			Write_MAX7219_Multi_Copy(Wing[Wing_ID-1].Matrix_CS_GPIOx, Wing[Wing_ID-1].Matrix_CS_GPIO_Pin, LED_Number, ShutDown, 0x00);	//进入掉电模式关闭显示
			HAL_GPIO_WritePin(Wing[Wing_ID-1].Beside_GPIOx, Wing[Wing_ID-1].Beside_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Wing[Wing_ID-1].Top_GPIOx , Wing[Wing_ID-1].Top_GPIO_Pin, GPIO_PIN_RESET);
			break;
		
		case ON:		//全部打开，点阵全显示
			Hit_cnt.Start_State=OFF;
			Write_MAX7219_Multi_Copy(Wing[Wing_ID-1].Matrix_CS_GPIOx, Wing[Wing_ID-1].Matrix_CS_GPIO_Pin, LED_Number, ShutDown, 0x01);	//关闭掉电模式
			Light_Up_All(Wing[Wing_ID-1].Matrix_CS_GPIOx, Wing[Wing_ID-1].Matrix_CS_GPIO_Pin);
			HAL_GPIO_WritePin(Wing[Wing_ID-1].Beside_GPIOx, Wing[Wing_ID-1].Beside_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Wing[Wing_ID-1].Top_GPIOx , Wing[Wing_ID-1].Top_GPIO_Pin, GPIO_PIN_SET);
			break;
		
		case HIT:		//顶部灯条打开，两侧灯条关闭，点阵流水箭头，开始计时
			Write_MAX7219_Multi_Copy(Wing[Wing_ID-1].Matrix_CS_GPIOx, Wing[Wing_ID-1].Matrix_CS_GPIO_Pin, LED_Number, Display_Test, 0x00);	//关闭测试模式
			Write_MAX7219_Multi_Copy(Wing[Wing_ID-1].Matrix_CS_GPIOx, Wing[Wing_ID-1].Matrix_CS_GPIO_Pin, LED_Number, ShutDown, 0x01);	//关闭掉电模式
			HAL_GPIO_WritePin(Wing[Wing_ID-1].Beside_GPIOx, Wing[Wing_ID-1].Beside_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Wing[Wing_ID-1].Top_GPIOx , Wing[Wing_ID-1].Top_GPIO_Pin, GPIO_PIN_SET);
			Hit_cnt.Wing_ID=Wing_ID;
			Hit_cnt.cnt=0;
			Hit_cnt.Shining_cnt=0;
			Hit_cnt.Start_State=ON;
			break;
		
		default:
				break;
	}
}
void Light_Up_Next_Wing(void)	//点亮下一个扇叶
{
	uint16_t Rand_Num=Rand_TIM(1,Wing_Num);
	while( Wing[Rand_Num].State!=OFF )
		Rand_Num=Rand_TIM(1,Wing_Num);
	Change_Work_State(Rand_Num,HIT);
}
void Wind_Init(void)	//全关，点亮一个扇叶
{
	for(uint8_t i=1; i<=Wing_Num; ++i)
		Change_Work_State(i,OFF);
//	Light_Up_Next_Wing();
}
void Wind_Test(void)
{
//	for(uint8_t i=1; i<=Wing_Num; ++i)		//全开
//		Change_Work_State(i,ON);
//	HAL_Delay(3000);
//	for(uint8_t i=1; i<=Wing_Num; ++i)		//全关
//		Change_Work_State(i,OFF);
//	HAL_Delay(500);
//	for(uint8_t i=1; i<=Wing_Num; ++i)		//击打
//	{
//		Change_Work_State(i,HIT);
//		HAL_Delay(1000);
//	}
//	for(uint8_t i=1; i<=Wing_Num; ++i)
//		Change_Work_State(i,OFF);
	
	Change_Work_State(1,OFF);
	HAL_Delay(500);
	Change_Work_State(1,HIT);
	HAL_Delay(3000);
}
void TIM_Hit_Cnt(void)
{
	if(Hit_cnt.Start_State==ON)
	{
		Pic_8_Display(Wing[Hit_cnt.Wing_ID-1].Matrix_CS_GPIOx, Wing[Hit_cnt.Wing_ID-1].Matrix_CS_GPIO_Pin);		//流水箭头显示
		++Hit_cnt.cnt;
		if(Hit_cnt.cnt>=25)		//超时
		{
			Hit_cnt.cnt=0;
			Hit_cnt.Start_State=OFF;
			Hit_cnt.Hit_Success_cnt=0;
			Hit_cnt.Shining_cnt=6;		//闪烁6/2=3次
		}
	}
	if(Hit_cnt.Shining_cnt>0)	//闪烁
	{
		if(Hit_cnt.Shining_cnt%2==1)
			for(uint8_t i=1; i<=Wing_Num; ++i)
				Change_Work_State(i,OFF);
		else
			for(uint8_t i=1; i<=Wing_Num; ++i)
				Change_Work_State(i,ON);
		--Hit_cnt.Shining_cnt;
	}
}
void Judge_Hit_Check(void)
{
	if(Hurt_Data_Update==TRUE)
	{
		if(RobotHurt.armor_id+1==Hit_cnt.Wing_ID)	//正确击打
		{
			Hit_cnt.cnt=0;
			Hit_cnt.Start_State=OFF;
			Hit_cnt.Shining_cnt=0;
			Change_Work_State(Hit_cnt.Wing_ID,ON);
			++Hit_cnt.Hit_Success_cnt;
			if(Hit_cnt.Hit_Success_cnt<Wing_Num)		//判断有没有全部全部点亮
				Light_Up_Next_Wing();		//没有就继续点亮
		}
		else	//错误击打
		{
			Hit_cnt.cnt=0;
			Hit_cnt.Start_State=OFF;
			Hit_cnt.Hit_Success_cnt=0;
			Hit_cnt.Shining_cnt=6;		//闪烁3次
		}
	}
}
