#ifndef __RESPOND_H
#define __RESPOND_H
#include "system.h"
#include "move.h"

#define Drive_set	 		0x01
#define VEL_set 			0x02
#define ACC_set 			0x03
#define SITE_set 			0x04

#define VEL_req  			0x05
#define Current_req 		0x06
#define Gyro_req 			0x07
#define UWB_req 			0x08
#define UNDM_req 			0x09

#define Heart_Check 		0x10
#define Error_Alarm 		0x11
#define VEL_res				0x12
#define Odom_res			0x13

#define IPC_SET				0xFF
#define IPC_REQ				0xFC
#define IPC_RES				0xFE
void IPC_Set(u8 *msg, AGV_MODEL *agv);
void IPC_Req(u8 cmd, u8 *msg, AGV_MODEL *agv);
void IPC_Res(u8 cmd, u8 *msg, AGV_MODEL *agv);

#endif


