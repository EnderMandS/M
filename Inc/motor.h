#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f1xx.h"
#include "main.h"
#include "pid.h"

typedef struct
{
	uint16_t angle;	//读取角度
	int16_t speed;	//读取速度
	uint16_t current;	//读取电流
	uint8_t temperature;	//读取温度
	uint16_t Angle_Set;	//设置角度
	uint16_t Angle_Area_Min;	//角度最小值
	uint16_t Angle_Area_Max;	//角度最大值
	int16_t Speed_Set;	//设置速度
	int16_t Vot_Cur_Set;	//设置电压电流
	int16_t Speed_Cash[5];	//读取速度缓冲区
	uint8_t Speed_Cash_Write_Cnt;	//读取速度缓冲区计数
	fp32 Speed_PID[3];	//速度环PID
	fp32 Angle_PID[3];	//角度环PID
	pid_type_def PID_Struct_Motor;	//速度环PID结构体
	pid_type_def PID_Struct_Angle;	//角度环PID结构体
}motor_data;

#define Motor_Base 0x200

extern motor_data M_data[];

void Motor_6020_Init(uint8_t Motor_ID);		//6020初始化
void Motor_Ctrl(void);	//向CAN总线发送电机控制数据
void Motoe_Turn_To(uint8_t Motor_ID, uint16_t Angle);	//转到指定角度
void Motor_Check_Angle(uint8_t Motor_ID);	//判断角度是否为设置角度，不在则转向
void Motor_Check_Speed(uint8_t Motor_ID);	//判读速度是否为设置速度，不在则变速
void Motor_Check(uint8_t Motor_ID);
void Speed_Change(void);

#endif
