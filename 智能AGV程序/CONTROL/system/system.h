#ifndef __SYSTEM_H
#define __SYSTEM_H	 
#include "sys.h" 
#include <stdint.h>
#include <math.h>
#include <string.h>  
#include "usart.h"
#include "delay.h"

//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS

#include "includes.h"					//ucos ʹ��	  
#endif
#define ABS(a) ((a > 0) ? a : -a)

#define DEBUG_MODE		0

extern OS_SEM	CAN_SEM;
extern OS_FLAG_GRP UNDM_FLAG_GRP;
extern CanRxMsg RxMessage;
extern OS_TCB RESPOND_TaskTCB;	

void My_Init(void);
void main_task(void *p_arg);
void sport_task(void *p_arg);
void can_task(void *p_arg);
void respond_task(void *p_arg);
#endif
