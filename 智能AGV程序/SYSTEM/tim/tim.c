#include "tim.h"


void UNDM_TIM_Init()
{
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TimeBaseInitStructure.TIM_ClockDivision=0;
	TimeBaseInitStructure.TIM_Prescaler=420-1;	 //������ʱ��Ƶ��=f_APB1CLK/420=100KHz;ÿ10us�������仯һ��
	TimeBaseInitStructure.TIM_Period=0xFFFF;
	TIM_TimeBaseInit(TIM6,&TimeBaseInitStructure);
//	TIM_ARRPreloadConfig(TIM6,ENABLE);

}

