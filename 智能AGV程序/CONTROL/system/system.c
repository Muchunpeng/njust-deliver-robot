#include "system.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "move.h"
#include "servo.h"
#include "iwdg.h"
#include "can.h"
#include "respond.h"
AGV_MODEL my_agv;

void My_Init()
{
    my_agv.left_wheel.ID = Lift_Wheel;
    my_agv.right_wheel.ID = Right_Wheel;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);    		//初始化延时函数
    uart_init(115200);
    MY_DMA_Init();
    LED_Init();					//初始化LED
    KEY_Init(); 				//按键初始化
    CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 3, CAN_Mode_Normal); //CAN初始化环回模式,波特率1Mbps
    Servo_Init(NMT_Operate);
   IWDG_Init(5,500); 			//与分频数为64,重载值为500,溢出时间为1s
    printf("硬件初始化完成\r\n");
}


void main_task(void *p_arg)
{
    OS_ERR err;
    static u8 time;
    while(1)
    {
        if(time < 10)	time++;
        else time = 0, LED0 = ~LED0, LED1 = ~LED1;
        OSTimeDlyHMSM(0, 0, 0, 20, OS_OPT_TIME_PERIODIC, &err); //延时20ms
    }
}

void can_task(void *p_arg)
{
    OS_ERR err;
    while(1)
    {
        OSTimeDlyHMSM(0, 0, 0, 20, OS_OPT_TIME_PERIODIC, &err); //延时20ms
    }
}


void respond_task(void *p_arg)
{
    static u8 flag;
    u8 *rx;
    u8 tx[12];
    OS_ERR err;
    OS_MSG_SIZE size;
    while(1)
    {
        //请求消息
        rx = OSTaskQPend((OS_TICK		)10,
                         (OS_OPT		)OS_OPT_PEND_BLOCKING,
                         (OS_MSG_SIZE *)&size,
                         (CPU_TS *		)0,
                         (OS_ERR *      )&err );
        if(size)
        {
            if(rx[0] == IPC_SET)
            {
                IPC_Set(rx, &my_agv);
            }
            else if(rx[0] == IPC_REQ)
            {
                IPC_Req(rx[2], tx, &my_agv);
            }
        }
        else
        {
			#if DEBUG_MODE
			printf("里程计信息	%d	%d\r\n",my_agv.left_wheel.actV,my_agv.right_wheel.actV);
			printf("速度　信息	%d	%d\r\n",my_agv.left_wheel.odom,my_agv.right_wheel.odom);
			#else
            if(flag)	IPC_Res(VEL_res, tx, &my_agv);
            else
				IPC_Res(Odom_res, tx, &my_agv);
            flag = !flag;
			#endif
        }
        OSSched	();						//发起任务调度
    }
}
void sport_task(void *p_arg)
{
    OS_ERR err;

    while(1)
    {
		Car_Control();
        OSTimeDlyHMSM(0, 0, 0, 20, OS_OPT_TIME_PERIODIC, &err); //延时20ms
    }
}




