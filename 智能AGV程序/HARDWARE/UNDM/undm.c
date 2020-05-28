#include "undm.h"
#define Sonic_Speed		340			//声速,单位m/s
#define Trig_Interval	50			//触发超声波测距时间间隔,单位ms
#define Error_Time		50
#define UNDM_TIMER	TIM6

USSensorDef g_USSensor[4];

void undm_task(void *p_arg)
{
    OS_ERR err;
    OS_FLAGS flag;
    while(1)
    {
        flag = OSFlagPend(&UNDM_FLAG_GRP,
                          US_TRIG_REQ ,
                          0,
                          OS_OPT_PEND_FLAG_SET_ANY,
                          0, &err);
        if(err == OS_ERR_NONE)
        {
            if(flag & US_TRIG_REQ)
            {
                while(!(flag & US_STOP_TRIG_REQ))
                {
                    for(int i = 0; i <= 3; ++i)
                    {
                        us_Measure(i);
                        flag = OSFlagPend(&UNDM_FLAG_GRP,
                                          US_STOP_TRIG_REQ,
                                          Trig_Interval,
                                          OS_OPT_PEND_FLAG_SET_ANY,
                                          0, &err);
                    }
                    OSFlagPost(&UNDM_FLAG_GRP, US_TRIG_DONE, OS_OPT_POST_FLAG_SET, &err);
                }
                OSFlagPost(&UNDM_FLAG_GRP, US_TRIG_REQ, OS_OPT_POST_FLAG_CLR, &err);
            }
            OSFlagPost(&UNDM_FLAG_GRP, US_STOP_TRIG_REQ, OS_OPT_POST_FLAG_CLR, &err);
        }
    }

}
/**************************************************************************
函数功能：等待超声波信号返回
入口参数：等待条件、等待时间
返回  值：成功：1
		　失败：0
备注：
**************************************************************************/
u8 UNDM_Wait(u8 cond, u8 time)
{
    while(cond)
    {
        time--;
        if(!time)
            return 0;
        delay_us(100);
    }
    return 1;
}
/**************************************************************************
函数功能：超声波传感器驱动
入口参数：发射脚、接受脚、最大响应时间
返回  值：成功：1
		　失败：0
备注：超声波"触发-等待响应-计时-停止计时"过程
**************************************************************************/
u8 Trig_Echo(u8 Tx, u8 Rx, u8 pErrorTime)
{
    u8 status = 1;
    Tx = 1;
    delay_us(15);
    Tx = ~Tx;
    status = UNDM_Wait(Rx, pErrorTime);
    if(!status)return status;
    TIM_SetCounter(UNDM_TIMER, 0);
    TIM_Cmd(UNDM_TIMER, ENABLE);
    status = UNDM_Wait(Rx, pErrorTime);
    TIM_Cmd(UNDM_TIMER, DISABLE);		
    return status;
}

/**************************************************************************
函数功能：分时复用驱动多传感器
入口参数：超声波ID
返回  值：成功：1
		　失败：0
备注：超声波"触发-等待响应-计时-停止计时"过程
**************************************************************************/
u8 us_Measure(uint8_t ID)
{
    OS_ERR err;
    uint8_t status = 1;

    switch(ID)
    {
    case 0:
        Trig_Echo(US_FR_T, US_FR_R, Error_Time);
        break;
    case 1:
        Trig_Echo(US_FL_T, US_FL_R, Error_Time);
        break;
    case 2:
        Trig_Echo(US_BR_T, US_BR_R, Error_Time);
        break;
    case 3:
        Trig_Echo(US_BL_T, US_BL_R, Error_Time);
        break;
    default:
        break;
    }

    if(!status)return status;
    g_USSensor[ID].errCode = 0;
    g_USSensor[ID].range = TIM_GetCounter(UNDM_TIMER) * Sonic_Speed / 400.0;	//计算距离，单位mm
    g_USSensor[ID].TS = OSTimeGet(&err);																//系统时间
    return status;
}

void UNDM_Init()
{
    GPIO_InitTypeDef GPIOInitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIOInitStructure.GPIO_Mode = GPIO_Mode_OUT;	//输出
    GPIOInitStructure.GPIO_OType = GPIO_OType_PP;
    GPIOInitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
    GPIOInitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉输出
    GPIOInitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIOInitStructure);

    GPIOInitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
    GPIO_Init(GPIOC, &GPIOInitStructure);

    GPIOInitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIOInitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	//下拉输入
    GPIOInitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
    GPIO_Init(GPIOD, &GPIOInitStructure);

    GPIOInitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_3;
    GPIO_Init(GPIOC, &GPIOInitStructure);

    GPIO_ResetBits(GPIOD, GPIO_Pin_8 | GPIO_Pin_10);
    GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_13);
}



