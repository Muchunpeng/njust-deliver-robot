#ifndef __USBH_HID_GAMEPAD_H
#define __USBH_HID_GAMEPAD_H

#include "usbh_hid_core.h"
 

//FC游戏手柄数据格式定义
//1,表示没有按下,0表示按下.
typedef struct _FC_GamePad_TypeDef 
{
		u8 a;		//A键
		u8 b;		//B键
		u8 c;		//C键
		u8 d;		//D键
		u8 l1;		//A键
		u8 l2;		//B键
		u8 r1;		//C键
		u8 r2;		//D键
		u8 select;//Select键
		u8 start; //Start键
		u8 up;	//上
		u8 down;	//下
		u8 left;	//左
		u8 right; //右
}FC_GamePad_TypeDef ; 
extern FC_GamePad_TypeDef fcpad;	//fc游戏手柄1
extern FC_GamePad_TypeDef fcpad1;	//fc游戏手柄2
extern HID_cb_TypeDef HID_GAMEPAD_cb; 


 

void USR_GAMEPAD_Init(void);
void USR_GAMEPAD_ProcessData(uint8_t data);
 


#endif 






















