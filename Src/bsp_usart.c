#include "bsp_usart.h"
#include "motor.h"
#include <cstring>
#include "usart.h"
#include "bsp_dma.h"
#include "judge.h"
#include "max7219.h"
#include "rolling.h"

uint8_t ch;
uint8_t ch_r;
int16_t Tr_Data[5]={0};

int fputc(int c, FILE * f)
{
	ch=c;
	HAL_UART_Transmit(&huart1,&ch,1,1000);
	return c;
}
int fgetc(FILE * F)
{
	HAL_UART_Receive (&huart1,&ch_r,1,0xffff);
	return ch_r;
}

//山外USART1
void usart1_send_char(uint8_t c)
{
	HAL_UART_Transmit(&huart1,&c,1,1000);
}
void sw_send_data(uint8_t *data,uint8_t len)
{  
	usart1_send_char(ZHENG);
	usart1_send_char(~ZHENG);
	for(uint8_t i=0;i<len;++i)
	{
		usart1_send_char(*data);
		data++;
	}
	usart1_send_char(~ZHENG);
	usart1_send_char(ZHENG);
}
void SW_Send_Motor_Data(uint8_t Motor_ID)
{
	Tr_Data[0]=(int16_t)M_data[Motor_ID-1].Angle_Set;	//角度设置 360
	Tr_Data[1]=M_data[Motor_ID-1].Vot_Cur_Set;	//电压电流设置
	Tr_Data[2]=(int16_t)(M_data[Motor_ID-1].angle*1.0/8192*360);	//当前角度 360
	Tr_Data[3]=(int16_t)(M_data[Motor_ID-1].Angle_Set*1.0/360.0*8192);	//角度设置 8192
	Tr_Data[4]=(int16_t)M_data[Motor_ID-1].angle;	//当前角度 8192
	sw_send_data( (uint8_t*)(&Tr_Data[0]),2*5);
}
void UART_IdleRxCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		memcpy(&rx_judge_buf,Judgement_Buf,200);//数据长度
		Judge_Read_Data(Judgement_Buf);
	}
	else if(huart == &huart1)
	{
		Words_To_String(Rolling.String_BUF,Rolling_Words_BUF);		//写入字符缓冲区
		if(Interrupt_Display_Flag==1)
			Interrupt_Display(GPIOA,GPIO_PIN_6);		//立即显示
	}
}
