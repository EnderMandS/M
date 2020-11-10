#include "motor.h"
#include "bsp_can.h"
#include <math.h>

motor_data M_data[11];
double Time_Now=0;	//s

void Motor_6020_Init(uint8_t Motor_ID)
{
	M_data[Motor_ID-1].Angle_Area_Min = 0;
	M_data[Motor_ID-1].Angle_Area_Max = 360;
	Motoe_Turn_To(Motor_ID,180);
}
void Motor_Ctrl(void)
{
	if(M_data[0].Vot_Cur_Set!=0 || M_data[1].Vot_Cur_Set!=0 || M_data[2].Vot_Cur_Set!=0 || M_data[3].Vot_Cur_Set!=0)
		CAN_cmd_chassis1(M_data[0].Vot_Cur_Set,M_data[1].Vot_Cur_Set,M_data[2].Vot_Cur_Set,M_data[3].Vot_Cur_Set);
	if(M_data[4].Vot_Cur_Set!=0 || M_data[5].Vot_Cur_Set!=0 || M_data[6].Vot_Cur_Set!=0 || M_data[7].Vot_Cur_Set!=0)
		CAN_cmd_chassis2(M_data[4].Vot_Cur_Set,M_data[5].Vot_Cur_Set,M_data[6].Vot_Cur_Set,M_data[7].Vot_Cur_Set);
	if(M_data[8].Vot_Cur_Set!=0 || M_data[9].Vot_Cur_Set!=0 || M_data[10].Vot_Cur_Set!=0)
		CAN_cmd_chassis3(M_data[8].Vot_Cur_Set,M_data[9].Vot_Cur_Set,M_data[10].Vot_Cur_Set);
}
void Motoe_Turn_To(uint8_t Motor_ID, uint16_t Angle)
{
	if(Angle>360)
		return;
	if(Angle<M_data[Motor_ID-1].Angle_Area_Min)
		M_data[Motor_ID-1].Angle_Set = M_data[Motor_ID-1].Angle_Area_Min;
	else if(Angle>M_data[Motor_ID-1].Angle_Area_Max)
		M_data[Motor_ID-1].Angle_Set = M_data[Motor_ID-1].Angle_Area_Max;
	else
		M_data[Motor_ID-1].Angle_Set = Angle;
}
int16_t Angle_Change(uint16_t Angle_Now,uint16_t Angle_Dst)
{
	uint16_t Return_Temp=0;
	if(Angle_Dst-Angle_Now>=4096)
		Return_Temp = -(8192-Angle_Dst+Angle_Now);
	else if(Angle_Dst-Angle_Now<=-4096)
		Return_Temp = 8192-Angle_Now+Angle_Dst;
	else
		Return_Temp = Angle_Dst-Angle_Now;
	return Return_Temp;
}
void Motor_Check_Angle(uint8_t Motor_ID)
{
	int16_t Angle_Send_To_PID = Angle_Change(M_data[Motor_ID-1].angle, (uint16_t)(M_data[Motor_ID-1].Angle_Set*1.0/360.0*8192));
	M_data[Motor_ID-1].Speed_Set = (int16_t)PID_calc( &M_data[Motor_ID-1].PID_Struct_Angle, 0, Angle_Send_To_PID);
	M_data[Motor_ID-1].Vot_Cur_Set = (int16_t)PID_calc( &M_data[Motor_ID-1].PID_Struct_Motor, M_data[Motor_ID-1].speed, M_data[Motor_ID-1].Speed_Set );
}
void Motor_Check_Speed(uint8_t Motor_ID)
{
	M_data[Motor_ID-1].Vot_Cur_Set=(int16_t)PID_calc( &M_data[Motor_ID-1].PID_Struct_Motor, M_data[Motor_ID-1].speed, M_data[Motor_ID-1].Speed_Set );
}
void Motor_Check(uint8_t Motor_ID)
{
	switch (Motor_ID)
	{
		case 1:
			
			break;
		case 2:
			
			break;
		case 3:
			
			break;
		case 4:
			Motor_Check_Speed(Motor_ID);
			break;
		case 5:
			
			break;
		case 6:
			
			break;
		case 7:
			
			break;
		case 8:
			
			break;
		case 9:
			Motor_Check_Angle(Motor_ID);
			break;
		case 10:
			
			break;
		case 11:
			
			break;
		case 12:
			
			break;
		
		default:
			break;
	}
}
void Speed_Change(void)
{
	Time_Now+=0.001;
	M_data[4-1].Speed_Set=(int16_t)(19.0*0.785*sin(1.884*Time_Now)+1.305);
}
