#ifndef __USBH_HID_GAMEPAD_H
#define __USBH_HID_GAMEPAD_H

#include "usbh_hid_core.h"
 

//FC��Ϸ�ֱ����ݸ�ʽ����
//1,��ʾû�а���,0��ʾ����.
typedef struct _FC_GamePad_TypeDef 
{
		u8 a;		//A��
		u8 b;		//B��
		u8 c;		//C��
		u8 d;		//D��
		u8 l1;		//A��
		u8 l2;		//B��
		u8 r1;		//C��
		u8 r2;		//D��
		u8 select;//Select��
		u8 start; //Start��
		u8 up;	//��
		u8 down;	//��
		u8 left;	//��
		u8 right; //��
}FC_GamePad_TypeDef ; 
extern FC_GamePad_TypeDef fcpad;	//fc��Ϸ�ֱ�1
extern FC_GamePad_TypeDef fcpad1;	//fc��Ϸ�ֱ�2
extern HID_cb_TypeDef HID_GAMEPAD_cb; 


 

void USR_GAMEPAD_Init(void);
void USR_GAMEPAD_ProcessData(uint8_t data);
 


#endif 






















