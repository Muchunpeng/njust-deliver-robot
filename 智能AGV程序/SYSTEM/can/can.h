#ifndef __CAN_H
#define __CAN_H	 
#include "system.h"
#include "servo.h"

							    

u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN≥ı ºªØ
u8 CAN1_Send_Msg(u16 id,u8 *msg, u8 len);
u16 CAN_Check(CanRxMsg *rxmsg,AGV_MODEL *agv);


#endif

















