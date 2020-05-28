#ifndef __SERVO_H
#define __SERVO_H
#include "system.h"

#define CAN_WaitTime				20
//左右两轮伺服电机ID
#define Lift_Wheel					0x01
#define Right_Wheel					0X02

#define SDO_WRITE					0x0600
#define SDO_READ					0x0580
#define TPDO1						0x0180
#define TPDO2						0x0280
#define TPDO3						0x0380
#define TPDO4						0x0480
#define Heart_Beat					0x0700

//SDO模式下，索引标识
#define ControlWord 				0x6040
#define StatusWord 					0x6041
#define Modes_Of_Operation			0x6060
#define Position_Value_Calaculated 	0x6064
#define Velocity_Value_Calaculated	0x606C
#define Current_Actual_Value 		0x6078
#define Profile_P2P_Velocity		0x6081
#define Profile_Acceleration 		0x6083
#define Profile_Deceleration 		0x6084
#define Profile_Velocity			0x60FF
//命令字的字节数
#define Set_Feedback 				0x60
#define	Set_One						0x2F
#define Set_Two						0x2B
#define Set_Three					0x27
#define Set_Four					0x23

#define Read_Data					0x40
#define	Back_One					0x4F
#define Back_Two					0x4B
#define Back_Three					0x47
#define Back_Four					0x43

#define Error_Data					0x80
//NMT状态管理指令
#define NMT_Manage					0x000
#define	NMT_Operate					0x01
#define	NMT_Stop					0x02
#define	NMT_Pre_Operate				0x80
#define	NMT_Reset_Use				0x81
#define	NMT_Reset_Com				0x82
#define All_Node					0x00
//NMT心跳返回
#define Back_Operate				0x05
#define Back_Pre					0x7F

#define Position_Mode				0x01
#define	Velocity_Mode				0x03

#define ShutDown 					0x06
#define Swith_On					0x07
#define Enable_Operate				0x0F
#define HALT						0x01
#define MOVE						0x00

typedef struct
{
    u8 ID;
    u8 mode;
    u8 enable;
    u8 halt;
    u8 nmt;
    u8 status;
    int16_t tarV;
    int32_t tarPos;

    int16_t actV;
    int32_t actPos;

    float actCUR;
    float actTOR;
    int32_t odom;
    int32_t last_odom;
	int32_t incre_odom;

} ServoMotor;
typedef struct
{

    u8 mode;
    u8 status;
    float tarV;
    float tarW;
    float tarACC;
    float tarDEC;
    int32_t tarPos;

    ServoMotor left_wheel;
    ServoMotor right_wheel;
    u16 q[4];
    u16 uwb[3];
    u16 undm[4];

} AGV_MODEL;

extern AGV_MODEL my_agv;
u8 Servo_Mode(u16 id, u8 mode);
u8 SDO_Write(u16 id, u16 index, int32_t data);
u8 SDO_Write_Back(u16 id, u16 index, CanRxMsg *rxmsg);
int32_t SDO_Read(u16 id, u16 index);
u8 Servo_Init(u8 mode);
u8 Select_Len(u16 index, u16 rw);

u8 PDO_Back(CanRxMsg *can, ServoMotor *motor);
int32_t SDO_Read_Back(u16 id, u16 index, CanRxMsg *rxmsg);
#endif

