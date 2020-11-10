#ifndef __JUDGE_H__
#define __JUDGE_H__

#include "main.h"
#include "stdbool.h"
#include "crc.h"
#include "bsp_dma.h"

#define		JUDGE_VERSION	20

#if JUDGE_VERSION==20

#define	LEN_HEADER		5	//帧头长度
#define	LEN_CMDID		2	//命令码ID长度
#define	LEN_TAIL		2	//帧尾长度

//数据帧起始字节，固定值为 0xA5
#define		JUDGE_FRAME_HEADER		(0xA5)

typedef enum 
{
	FRAME_HEADER         = 0,  //帧头偏移为零，共5字节
	CMD_ID               = 5, //偏移为5，共2字节
	DATA                 = 7, //偏移为7，字节数为N
	
}JudgeFrameOffset;

//5字节帧头,各自的偏移位置
typedef enum
{
	SOF          = 0,//起始位
	DATA_LENGTH  = 1,//帧内数据长度,根据这个来获取数据长度
	SEQ          = 3,//包序号
	CRC8         = 4 //CRC8
	
}FrameHeaderOffset;

/***************命令码ID********************/

/* 

	ID: 0x0001  Byte:  3    比赛状态数据       						发送频率 1Hz      
	ID: 0x0002  Byte:  1    比赛结果数据         					比赛结束后发送      
	ID: 0x0003  Byte:  32   比赛机器人血量数据  		 			1Hz发送  
	ID：0x0004	Byte:  3	飞镖发射状态							飞镖发射时发送
	ID：0x0005	Byte:  3	人工智能挑战赛加成与惩罚区状态			 1HZ发送
	ID: 0x0101  Byte:  4    场地事件数据   							1HZ发送
	ID: 0x0102  Byte:  4    场地补给站动作标识数据    				动作改变后发送 
	ID: 0x0104	Byte:  2	裁判警告数据							警告发生后发送
	ID: 0x0105	Byte:  1	飞镖发射口倒计时						1HZ发送
	ID: 0X0201  Byte: 18    机器人状态数据        					10Hz
	ID: 0X0202  Byte: 16    实时功率热量数据   						50Hz       
	ID: 0x0203  Byte: 16    机器人位置数据           				10Hz
	ID: 0x0204  Byte:  1    机器人增益数据           				1HZ
	ID: 0x0205  Byte:  3    空中机器人能量状态数据,只有空中机器人主控发 10HZ
	ID: 0x0206  Byte:  1    伤害状态数据           					伤害发生后发送
	ID: 0x0207  Byte:  6    实时射击数据           					子弹发射后发送
	ID: 0X0208	Byte:  2    弹丸剩余发射数，仅空中机器人，哨兵机器人以及 ICRA 机器人发送 1HZ
	ID:	0X0209	Byte:  4	机器人RFID状态							1HZ
	ID:	0x020A	Byte:  12	飞镖机器人客户端指令数据				10HZ
	ID: 0x0301  Byte:  n    机器人间交互数据           	发送方触发发送,10Hz
	
*/
//命令码ID,用来判断接收的是什么数据
typedef enum
{ 
	ID_game_state       			= 0x0001,//比赛状态数据
	ID_game_result 	   				= 0x0002,//比赛结果数据
	ID_game_robot_blood       		= 0x0003,//比赛机器人血量数据
	ID_darts_took_off				= 0x0004,//飞镖发射状态
	ID_game_icra_buff				= 0x0005,//人工智能挑战赛加成与惩罚区状态
	ID_event_data  					= 0x0101,//场地事件数据 
	ID_supply_projectile_action   	= 0x0102,//场地补给站动作标识数据
	ID_judge_warning				= 0x0104,//裁判警告
	ID_dart_take_off_count_down		= 0x0105,//飞镖发射口倒计时
	ID_game_robot_state    			= 0x0201,//机器人状态数据
	ID_power_heat_data    			= 0x0202,//实时功率热量数据
	ID_game_robot_pos        		= 0x0203,//机器人位置数据
	ID_buff_musk					= 0x0204,//机器人增益数据
	ID_aerial_robot_energy			= 0x0205,//空中机器人能量状态数据
	ID_robot_hurt					= 0x0206,//伤害状态数据
	ID_shoot_data					= 0x0207,//实时射击数据
	ID_bullet_surplus				= 0x0208,//弹丸剩余发射数
	ID_robot_rfid_state				= 0x0209,//机器人RFID状态
	ID_darts_user_based_order		= 0x020A, //飞镖客户端指令

} CmdID;

//命令码数据段长,根据官方协议来定义长度
typedef enum
{
	LEN_game_state       				=  3,	//0x0001
	LEN_game_result       				=  1,	//0x0002
	LEN_game_robot_HP       			=  32,	//0x0003
	LEN_darts_took_off					=  3, 	//0x0004
	LEN_game_icra_buff					=  3,	//0x0005
	LEN_event_data  					=  4,	//0x0101
	LEN_supply_projectile_action        =  3,	//0x0102
	LEN_judge_warning					=  2,	//0x0104
	LEN_dart_take_off_count_down		=  1,	//0x0105
	LEN_game_robot_state    			= 18,	//0x0201
	LEN_power_heat_data   				= 16,	//0x0202
	LEN_game_robot_pos        			= 16,	//0x0203
	LEN_buff_musk        				=  1,	//0x0204
	LEN_aerial_robot_energy        		=  3,	//0x0205
	LEN_robot_hurt        				=  1,	//0x0206
	LEN_shoot_data       				=  6,	//0x0207
	LEN_bullet_surplus					=  2,	//0x0208
	LEN_robot_rfid_state				=  4,   //0x0209
	LEN_darts_user_based_order			=  12,   //0x020A
} JudgeDataLength;


/* 自定义帧头 */
typedef __packed struct
{
	uint8_t  SOF;
	uint16_t DataLength;
	uint8_t  Seq;
	uint8_t  CRC8;
	
} xFrameHeader;

// 比赛状态数据：0x0001。发送频率：1Hz，发送范围：所有机器人。
typedef __packed struct
{
 uint8_t game_type : 4;
 uint8_t game_progress : 4;
 uint16_t stage_remain_time;
} ext_game_status_t;

// 比赛结果数据：0x0002。发送频率：比赛结束后发送，发送范围：所有机器人。
typedef __packed struct
{
 uint8_t winner;
} ext_game_result_t;


// 机器人血量数据：0x0003。发送频率：1Hz，发送范围：所有机器人。
typedef __packed struct
{
 uint16_t red_1_robot_HP;
 uint16_t red_2_robot_HP; 
 uint16_t red_3_robot_HP; 
 uint16_t red_4_robot_HP; 
 uint16_t red_5_robot_HP; 
 uint16_t red_7_robot_HP; 
 uint16_t red_outpost_HP;
 uint16_t red_base_HP; 
 uint16_t blue_1_robot_HP; 
 uint16_t blue_2_robot_HP; 
 uint16_t blue_3_robot_HP; 
 uint16_t blue_4_robot_HP; 
 uint16_t blue_5_robot_HP; 
 uint16_t blue_7_robot_HP; 
 uint16_t blue_outpost_HP;
 uint16_t blue_base_HP;
} ext_game_robot_HP_t;

// 飞镖发射状态：0x0004。发送频率：飞镖发射后发送，发送范围：所有机器人。
typedef __packed struct
{
 uint8_t dart_belong; 
 uint16_t stage_remaining_time; 
} ext_dart_status_t;

// 人工智能挑战赛加成与惩罚区状态：0x0005。发送频率：1Hz 周期发送，发送范围：所有机器人。
typedef __packed struct
{
 uint8_t F1_zone_status:1;
 uint8_t F1_zone_buff_debuff_status:3;
 uint8_t F2_zone_status:1;
 uint8_t F2_zone_buff_debuff_status:3; 
 uint8_t F3_zone_status:1;
 uint8_t F3_zone_buff_debuff_status:3; 
 uint8_t F4_zone_status:1;
 uint8_t F4_zone_buff_debuff_status:3; 
 uint8_t F5_zone_status:1;
 uint8_t F5_zone_buff_debuff_status:3; 
 uint8_t F6_zone_status:1;
 uint8_t F6_zone_buff_debuff_status:3;
} ext_ICRA_buff_debuff_zone_status_t;

//场地事件数据：0x0101。发送频率：1Hz 周期发送，发送范围：己方机器人。
typedef __packed struct
{
 uint32_t event_type;
} ext_event_data_t;

//补给站动作标识：0x0102。发送频率：动作触发后发送，发送范围：己方机器人。
typedef __packed struct
{
 uint8_t supply_projectile_id; 
 uint8_t supply_robot_id; 
 uint8_t supply_projectile_step; 
uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

// 裁判警告信息：cmd_id (0x0104)。发送频率：警告发生后发送，发送范围：己方机器人。
typedef __packed struct
{
 uint8_t level;
 uint8_t foul_robot_id; 
} ext_referee_warning_t;

// 飞镖发射口倒计时：cmd_id (0x0105)。发送频率：1Hz 周期发送，发送范围：己方机器人。
typedef __packed struct
{
 uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;

// 比赛机器人状态：0x0201。发送频率：10Hz，发送范围：单一机器人
typedef __packed struct
{
 uint8_t robot_id;
 uint8_t robot_level;
 uint16_t remain_HP;
 uint16_t max_HP;
 uint16_t shooter_heat0_cooling_rate;
 uint16_t shooter_heat0_cooling_limit;
 uint16_t shooter_heat1_cooling_rate;
 uint16_t shooter_heat1_cooling_limit;
 uint8_t shooter_heat0_speed_limit;
 uint8_t shooter_heat1_speed_limit;
 uint8_t max_chassis_power;
 uint8_t mains_power_gimbal_output : 1;
 uint8_t mains_power_chassis_output : 1;
 uint8_t mains_power_shooter_output : 1;
} ext_game_robot_status_t;

// 实时功率热量数据：0x0202。发送频率：50Hz，发送范围：单一机器人。
typedef __packed struct
{
 uint16_t chassis_volt; 
 uint16_t chassis_current; 
 float chassis_power; 
 uint16_t chassis_power_buffer; 
 uint16_t shooter_heat0; 
 uint16_t shooter_heat1; 
 uint16_t mobile_shooter_heat2;
} ext_power_heat_data_t;

// 机器人位置：0x0203。发送频率：10Hz，发送范围：单一机器人
typedef __packed struct
{
 float x;
 float y;
 float z;
 float yaw;
} ext_game_robot_pos_t;

// 机器人增益：0x0204。发送频率：1Hz 周期发送，发送范围：单一机器人。
typedef __packed struct
{
 uint8_t power_rune_buff;
}ext_buff_t;

// 空中机器人能量状态：0x0205。发送频率：10Hz，发送范围：单一机器人。
typedef __packed struct
{
 uint16_t energy_point;
 uint8_t attack_time;
} ext_aerial_robot_energy_t;

// 伤害状态：0x0206。发送频率：伤害发生后发送，发送范围：单一机器人
typedef __packed struct
{
 uint8_t armor_id : 4;
 uint8_t hurt_type : 4;
} ext_robot_hurt_t;


// 实时射击信息：0x0207。发送频率：射击后发送，发送范围：单一机器人。
typedef __packed struct
{
 uint8_t bullet_type;
 uint8_t bullet_freq;
 float bullet_speed;
} ext_shoot_data_t;

// 子弹剩余发射数：0x0208。发送频率：1Hz 周期发送，空中机器人，哨兵机器人以及 ICRA 机器人主控发送，发送范围：单一机器人
typedef __packed struct
{
 uint16_t bullet_remaining_num;
} ext_bullet_remaining_t;

// 机器人 RFID 状态：0x0209。发送频率：1Hz，发送范围：单一机器人。

typedef __packed struct
{
 uint32_t rfid_status;
} ext_rfid_status_t;


// 飞镖机器人客户端指令数据：0x020A。发送频率：10Hz，发送范围：单一机器人。
typedef __packed struct
{
 uint8_t dart_launch_opening_status;
 uint8_t dart_attack_target;
 uint16_t target_change_time;
 uint8_t first_dart_speed;
 uint8_t second_dart_speed;
 uint8_t third_dart_speed;
 uint8_t fourth_dart_speed;
 uint16_t last_dart_launch_time;
 uint16_t operate_launch_cmd_time;
} ext_dart_client_cmd_t;

/*****************系统数据定义**********************/
extern	ext_game_status_t       						GameState;					//0x0001
extern	ext_game_result_t            		GameResult;					//0x0002
extern	ext_game_robot_HP_t          		GameRobotHP;				//0x0003
extern	ext_dart_status_t					DartStatus;					//0x0004
extern	ext_ICRA_buff_debuff_zone_status_t  IcraBuffDebuffZoneStatus;	//0x0005
extern	ext_event_data_t        			EventData;					//0x0101
extern	ext_supply_projectile_action_t		SupplyProjectileAction;		//0x0102
extern	ext_referee_warning_t				RefereeWarning;				//0x0104
extern	ext_dart_remaining_time_t			DartRemainingTime;			//0x0105
extern	ext_game_robot_status_t				GameRobotStat;				//0x0201
extern	ext_power_heat_data_t		  		PowerHeatData;				//0x0202
extern	ext_game_robot_pos_t				GameRobotPos;				//0x0203
extern	ext_buff_t						BuffMusk;					//0x0204
extern	ext_aerial_robot_energy_t			AerialRobotEnergy;			//0x0205
extern	ext_robot_hurt_t					RobotHurt;					//0x0206
extern	ext_shoot_data_t					ShootData;					//0x0207
extern	ext_bullet_remaining_t				BulletRemaining;			//0x0208
extern	ext_rfid_status_t					RfridStatus;				//0x0209
extern	ext_dart_client_cmd_t				DartClientCmd;				//0x020A
extern	xFrameHeader             			FrameHeader;		//发送帧头信息

extern bool Hurt_Data_Update;
extern uint8_t rx_judge_buf[];

#define JUDGEMENT_BUF_LEN      200

void Dma_UsartIdleHanlder(UART_HandleTypeDef *huart,uint16_t Size);
bool Judge_Read_Data(uint8_t *ReadFromUsart);
void UART_IdleRxCallback(UART_HandleTypeDef *huart);

HAL_StatusTypeDef Bsp_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef Bsp_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

#endif

#endif
