#include "tim.h"


void UNDM_TIM_Init()
{
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TimeBaseInitStructure.TIM_ClockDivision=0;
	TimeBaseInitStructure.TIM_Prescaler=420-1;	 //计数器时钟频率=f_APB1CLK/420=100KHz;每10us计数器变化一次
	TimeBaseInitStructure.TIM_Period=0xFFFF;
	TIM_TimeBaseInit(TIM6,&TimeBaseInitStructure);
//	TIM_ARRPreloadConfig(TIM6,ENABLE);

}

