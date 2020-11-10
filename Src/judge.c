#include "judge.h"
#include "string.h"
#include "usart.h"

extern UART_HandleTypeDef huart1;
uint8_t rx_judge_buf[200];
extern uint8_t Judgement_Buf[];

/*****************系统数据定义**********************/
ext_game_status_t       							GameState;									//0x0001
ext_game_result_t            					GameResult;									//0x0002
ext_game_robot_HP_t          					GameRobotHP;								//0x0003
ext_dart_status_t											DartStatus;									//0x0004
ext_ICRA_buff_debuff_zone_status_t 		IcraBuffDebuffZoneStatus;		//0x0005
ext_event_data_t        							EventData;									//0x0101
ext_supply_projectile_action_t				SupplyProjectileAction;			//0x0102
ext_referee_warning_t									RefereeWarning;							//0x0104
ext_dart_remaining_time_t							DartRemainingTime;					//0x0105
ext_game_robot_status_t								GameRobotStat;							//0x0201
ext_power_heat_data_t		  						PowerHeatData;							//0x0202
ext_game_robot_pos_t									GameRobotPos;								//0x0203
ext_buff_t														BuffMusk;										//0x0204
ext_aerial_robot_energy_t							AerialRobotEnergy;					//0x0205
ext_robot_hurt_t											RobotHurt;									//0x0206
ext_shoot_data_t											ShootData;									//0x0207
ext_bullet_remaining_t								BulletRemaining;						//0x0208
ext_rfid_status_t											RfridStatus;								//0x0209
ext_dart_client_cmd_t									DartClientCmd;							//0x020A
xFrameHeader             							FrameHeader;								//发送帧头信息
/****************************************************/

bool Judge_Data_TF = FALSE;//裁判数据是否可用,辅助函数调用
uint8_t Judge_Self_ID;//当前机器人的ID
uint16_t Judge_SelfClient_ID;//发送者机器人对应的客户端ID


/**************裁判系统数据辅助****************/
uint16_t ShootNum;//统计发弹量,0x0003触发一次则认为发射了一颗
bool Hurt_Data_Update = FALSE;//装甲板伤害数据是否更新,每受一次伤害置TRUE,然后立即置FALSE,给底盘闪避用
#define BLUE  0
#define RED   1

bool Judge_Read_Data(uint8_t *ReadFromUsart)
{
	bool retval_tf = FALSE;//数据正确与否标志,每次调用读取裁判系统数据函数都先默认为错误
	
	uint16_t judge_length;//统计一帧数据长度 
	
	int CmdID = 0;//数据命令码解析
	
	/***------------------*****/
	//无数据包，则不作任何处理
	if (ReadFromUsart == NULL)
	{
		return -1;
	}
	
	//写入帧头数据,用于判断是否开始存储裁判数据
	memcpy(&FrameHeader, ReadFromUsart, LEN_HEADER);
	
	//判断帧头数据是否为0xA5
	if(ReadFromUsart[ SOF ] == JUDGE_FRAME_HEADER)
	{
		//帧头CRC8校验
		if (Verify_CRC8_Check_Sum( ReadFromUsart, LEN_HEADER ) == TRUE)
		{
			//统计一帧数据长度,用于CR16校验
			judge_length = ReadFromUsart[ DATA_LENGTH ] + LEN_HEADER + LEN_CMDID + LEN_TAIL;

			//帧尾CRC16校验
			if(Verify_CRC16_Check_Sum(ReadFromUsart,judge_length) == TRUE)
			{
				retval_tf = TRUE;//都校验过了则说明数据可用
				
				CmdID = (ReadFromUsart[6] << 8 | ReadFromUsart[5]); 
				//解析数据命令码,将数据拷贝到相应结构体中(注意拷贝数据的长度)
				switch(CmdID)
				{
					case ID_game_state:        			//0x0001 //比赛状态数据
						memcpy(&GameState, (ReadFromUsart + DATA), LEN_game_state);
					break;
					
					case ID_game_result:          		//0x0002 //比赛结果数据
						memcpy(&GameResult, (ReadFromUsart + DATA), LEN_game_result);
					break;
					
					case ID_game_robot_blood:       //0x0003 //比赛机器人血量
						memcpy(&GameRobotHP, (ReadFromUsart + DATA), LEN_game_robot_HP);
					break;
					
					case ID_darts_took_off:        //0X0004 //飞镖发射状态
						memcpy(&DartStatus,(ReadFromUsart + DATA),LEN_darts_took_off);
					break;

					case ID_game_icra_buff:			//0x0005 //人工智能挑战赛加成与惩罚区状态
						memcpy(&IcraBuffDebuffZoneStatus,(ReadFromUsart + DATA),LEN_game_icra_buff);
					break;
		
					case ID_event_data:    				//0x0101//场地事件//与空中有关
						memcpy(&EventData, (ReadFromUsart + DATA), LEN_event_data);
					break;
					
					case ID_supply_projectile_action:   //0x0102 //场地补给站动作标识数据//字节偏移量为3的数据（大小一个字节）是补弹数量 50：50 颗子弹； 
						memcpy(&SupplyProjectileAction, (ReadFromUsart + DATA), LEN_supply_projectile_action);
					break;

					case ID_judge_warning: //0X0104 //裁判系统警告
						memcpy(&RefereeWarning,(ReadFromUsart + DATA),LEN_judge_warning);
					break;
					
					case ID_dart_take_off_count_down: //0X0105//飞镖发射口倒计时
						memcpy(&DartRemainingTime,(ReadFromUsart + DATA),LEN_dart_take_off_count_down);
					break;

					case ID_game_robot_state:      		//0x0201//机器人状态数据
						memcpy(&GameRobotStat, (ReadFromUsart + DATA), LEN_game_robot_state);
					break;
					
					case ID_power_heat_data:      		//0x0202 //实时功率热量
						memcpy(&PowerHeatData, (ReadFromUsart + DATA), LEN_power_heat_data);
					break;
					
					case ID_game_robot_pos:      		//0x0203 //机器人位置
						memcpy(&GameRobotPos, (ReadFromUsart + DATA), LEN_game_robot_pos);
					break;
					
					case ID_buff_musk:      			//0x0204 //机器人增益
						memcpy(&BuffMusk, (ReadFromUsart + DATA), LEN_buff_musk);
					break;
					
					case ID_aerial_robot_energy:      	//0x0205 //空中机器人能量状态
						memcpy(&AerialRobotEnergy, (ReadFromUsart + DATA), LEN_aerial_robot_energy);
					break;
					
					case ID_robot_hurt:      			//0x0206 //伤害状态数据
						memcpy(&RobotHurt, (ReadFromUsart + DATA), LEN_robot_hurt);
						if(RobotHurt.hurt_type == 0 || RobotHurt.hurt_type == 5)//装甲板收到攻击伤害
						{	Hurt_Data_Update = TRUE;	}//装甲数据每更新一次则判定为受到一次装甲伤害
					break;
					
					case ID_shoot_data:      			//0x0207//实时射击数据
						memcpy(&ShootData, (ReadFromUsart + DATA), LEN_shoot_data);
						//JUDGE_ShootNumCount();//发弹量统
					break;

					case ID_bullet_surplus:			//0x0208//弹丸剩余发射数
						memcpy(&BulletRemaining,(ReadFromUsart + DATA),LEN_bullet_surplus);
					break;

					case ID_robot_rfid_state:		//0x0209//机器人RFID状态
						memcpy(&RfridStatus,(ReadFromUsart + DATA),LEN_robot_rfid_state);
					break;

					case ID_darts_user_based_order:		//0x020A//飞镖客户端指令数据
						memcpy(&DartClientCmd,(ReadFromUsart + DATA),LEN_darts_user_based_order);
					break;
				}
				//首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,用来判断一个数据包是否有多帧数据
//				if(*(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL) == 0xA5)
//				{
//					//如果一个数据包出现了多帧数据,则再次读取
//					Judge_Read_Data(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL);
//				}
			}
		}
		//首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,用来判断一个数据包是否有多帧数据
		if(*(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL) == 0xA5)
		{
			//如果一个数据包出现了多帧数据,则再次读取
			Judge_Read_Data(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL);
		}
	}
	
	if (retval_tf == TRUE)
	{
		Judge_Data_TF = TRUE;//辅助函数用
	}
	else		//只要CRC16校验不通过就为FALSE
	{
		Judge_Data_TF = FALSE;//辅助函数用
	}
	
	return retval_tf;//对数据正误做处理
}
