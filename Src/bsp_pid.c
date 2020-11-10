#include "bsp_pid.h"
#include "main.h"
#include "motor.h"

void PID_Bsp_Init()
{
	uint8_t Ctrl_ID = 9-1;
	
	M_data[Ctrl_ID].Speed_PID[0]=15;		//Kp
	M_data[Ctrl_ID].Speed_PID[1]=0;			//Ki
	M_data[Ctrl_ID].Speed_PID[2]=0;		//Kd
	
	M_data[Ctrl_ID].Angle_PID[0]=1;			//Kp
	M_data[Ctrl_ID].Angle_PID[1]=0;			//Ki
	M_data[Ctrl_ID].Angle_PID[2]=0;			//Kd
	
	PID_init(&(M_data[Ctrl_ID].PID_Struct_Motor), PID_POSITION, M_data[Ctrl_ID].Speed_PID, 30000, 30000);
	PID_init(&(M_data[Ctrl_ID].PID_Struct_Angle), PID_POSITION, M_data[Ctrl_ID].Angle_PID, 500, 500);
	
	Ctrl_ID=4-1;
	
	M_data[Ctrl_ID].Speed_PID[0]=5;		//Kp
	M_data[Ctrl_ID].Speed_PID[1]=0;			//Ki
	M_data[Ctrl_ID].Speed_PID[2]=0;		//Kd
	
	M_data[Ctrl_ID].Angle_PID[0]=5;			//Kp
	M_data[Ctrl_ID].Angle_PID[1]=0;			//Ki
	M_data[Ctrl_ID].Angle_PID[2]=0.5;			//Kd
	
	PID_init(&(M_data[Ctrl_ID].PID_Struct_Motor), PID_DELTA, M_data[Ctrl_ID].Speed_PID, 16384, 16384);
	PID_init(&(M_data[Ctrl_ID].PID_Struct_Angle), PID_DELTA, M_data[Ctrl_ID].Angle_PID, 500, 500);
}			

