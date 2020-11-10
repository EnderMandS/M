#include "bsp_can.h"
#include "bsp_usart.h"
#include "motor.h"
#include "can.h"
#include <math.h>

uint8_t Send_cnt=0;
uint8_t Send_Data[8];
CAN_TxHeaderTypeDef CAN_TxHeaderType;
CAN_RxHeaderTypeDef CAN_RxHeaderType;

void can_filter_init(void)
{
    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan, &can_filter_st);
    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}
void CAN_cmd_chassis1(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
	uint32_t send_mail_box;
	CAN_TxHeaderType.DLC=8;
	CAN_TxHeaderType.IDE=CAN_ID_STD;
	CAN_TxHeaderType.RTR=CAN_RTR_DATA;
	CAN_TxHeaderType.StdId=0x200;
	Send_Data[0] = motor1 >> 8;
	Send_Data[1] = motor1;
	Send_Data[2] = motor2 >> 8;
	Send_Data[3] = motor2;
	Send_Data[4] = motor3 >> 8;
	Send_Data[5] = motor3;
	Send_Data[6] = motor4 >> 8;
	Send_Data[7] = motor4;
	HAL_CAN_AddTxMessage(&hcan,&CAN_TxHeaderType,Send_Data,&send_mail_box);
}
void CAN_cmd_chassis2(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
	uint32_t send_mail_box;
	CAN_TxHeaderType.DLC=8;
	CAN_TxHeaderType.IDE=CAN_ID_STD;
	CAN_TxHeaderType.RTR=CAN_RTR_DATA;
	CAN_TxHeaderType.StdId=0x1ff;
	Send_Data[0] = motor1 >> 8;
	Send_Data[1] = motor1;
	Send_Data[2] = motor2 >> 8;
	Send_Data[3] = motor2;
	Send_Data[4] = motor3 >> 8;
	Send_Data[5] = motor3;
	Send_Data[6] = motor4 >> 8;
	Send_Data[7] = motor4;
	HAL_CAN_AddTxMessage(&hcan,&CAN_TxHeaderType,Send_Data,&send_mail_box);
}
void CAN_cmd_chassis3(int16_t motor1, int16_t motor2, int16_t motor3)
{
	uint32_t send_mail_box;
	CAN_TxHeaderType.StdId = 0x2ff;
	CAN_TxHeaderType.IDE = CAN_ID_STD;
	CAN_TxHeaderType.RTR = CAN_RTR_DATA;
	CAN_TxHeaderType.DLC = 0x08;
	Send_Data[0] = motor1 >> 8;
	Send_Data[1] = motor1;
	Send_Data[2] = motor2 >> 8;
	Send_Data[3] = motor2;
	Send_Data[4] = motor3 >> 8;
	Send_Data[5] = motor3;
	Send_Data[6] = 0;
	Send_Data[7] = 0;
	HAL_CAN_AddTxMessage(&hcan,&CAN_TxHeaderType,Send_Data,&send_mail_box);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t rx_data[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN_RxHeaderType, rx_data);
	switch(CAN_RxHeaderType.StdId)
	{
		case Motor_Base+1:		//0x201
		case Motor_Base+2:		//0x202
		case Motor_Base+3:		//0x203
		case Motor_Base+4:		//0x204
		case Motor_Base+5:		//0x205
		case Motor_Base+6:		//0x206
		case Motor_Base+7:		//0x207
		case Motor_Base+8:		//0x208
		case Motor_Base+9:		//0x209
		case Motor_Base+10:		//0x20A
		case Motor_Base+11:		//0x20B
		{
			uint8_t Mo_id = CAN_RxHeaderType.StdId - Motor_Base;
			M_data[Mo_id-1].angle = (uint16_t)((rx_data)[0] << 8 | (rx_data)[1]);
			M_data[Mo_id-1].speed = (uint16_t)((rx_data)[2] << 8 | (rx_data)[3]);
			M_data[Mo_id-1].current = (uint16_t)((rx_data)[4] << 8 | (rx_data)[5]);
			M_data[Mo_id-1].temperature = rx_data[6];
			++Send_cnt;
			if(Send_cnt==5)
			{
				Send_cnt=0;
				Motor_Check(Mo_id);
			}	
			break;
		}

		default:
			break;
	}
}
