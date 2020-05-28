#ifndef __UNDM_H
#define __UNDM_H
#include "system.h"

#define US_TRIG_DONE 			0x01
#define US_TRIG_REQ				0x02
#define US_STOP_TRIG_REQ		0x04

#define US_FR_T	PDout(8)
#define US_FR_R	PDin(9)

#define US_FL_T	PDout(10)
#define US_FL_R	PDin(11)

#define US_BR_T	PCout(0)
#define US_BR_R	PCin(13)

#define US_BL_T	PCout(2)
#define US_BL_R	PCin(3)

typedef struct
{
	float range;			//测距值
	uint32_t TS;			//时间戳
	uint8_t errCode;	//错误码;0~正常;1~错误
}USSensorDef;

void undm_task(void *p_arg);
void UNDM_Init(void);
u8 us_Measure(uint8_t ID);
u8 UNDM_Wait(u8 cond, u8 time);
u8 Trig_Echo(u8 Tx, u8 Rx, u8 pErrorTime);
#endif
