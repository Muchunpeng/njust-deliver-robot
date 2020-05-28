#include "system.h"
#include "led.h"


//UCOSIII中以下优先级用户程序不能使用，ALIENTEK
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()


//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小
#define START_STK_SIZE 		128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

#define MAIN_TASK_PRIO		7				//任务优先级
#define MAIN_STK_SIZE 		128				//任务堆栈大小	
OS_TCB MAIN_TaskTCB;						//任务控制块
CPU_STK MAIN_TASK_STK[MAIN_STK_SIZE];		//任务堆栈
void main_task(void *p_arg);

#define SPORT_TASK_PRIO		4				//任务优先级
#define SPORT_STK_SIZE 		128				//任务堆栈大-小	
OS_TCB SPORT_TaskTCB;						//任务控制块
CPU_STK SPORT_TASK_STK[SPORT_STK_SIZE];		//任务堆栈
void sport_task(void *p_arg);//任务函数

//CAN任务
#define CAN_TASK_PRIO		5				//任务优先级
#define CAN_STK_SIZE 		128				//任务堆栈大小
OS_TCB CAN_TaskTCB;							//任务控制块
CPU_STK CAN_TASK_STK[CAN_STK_SIZE];			//任务堆栈
void can_task(void *p_arg);

//数据上传任务
#define RESPOND_TASK_PRIO	6				//任务优先级
#define RESPOND_STK_SIZE 	128				//任务堆栈大小
#define RESPOND_TASK_Q_NUM		1			//发任务内建消息队列的长度
OS_TCB RESPOND_TaskTCB;						//任务控制块
CPU_STK RESPOND_TASK_STK[RESPOND_STK_SIZE];	//任务堆栈
void respond_task(void *p_arg);

#define UNDM_TASK_PRIO		10
#define UNDM_STK_SIZE		256
OS_TCB 	UNDM_TaskTCB;
CPU_STK UNDM_TASK_STK[UNDM_STK_SIZE];
void undm_task(void *p_arg);

OS_FLAG_GRP UNDM_FLAG_GRP;
OS_SEM	CAN_SEM;		//用于等待CAN返回消息

//主函数
int main(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    My_Init();
    OSInit(&err);		    //初始化UCOSIII
    OS_CRITICAL_ENTER();	//进入临界区
    //创建开始任务
    OSTaskCreate((OS_TCB * )&StartTaskTCB,		//任务控制块
                 (CPU_CHAR * )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void * )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE / 10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void * )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR * )&err);				//存放该函数错误时的返回值
    OS_CRITICAL_EXIT();	//退出临界区
    OSStart(&err);      //开启UCOSIII
}

//开始任务任务函数
void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
    //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
    OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif


    //创建MAIN任务
    OSTaskCreate((OS_TCB * )&MAIN_TaskTCB,
                 (CPU_CHAR * )"Main task",
                 (OS_TASK_PTR )main_task,
                 (void * )0,
                 (OS_PRIO	  )MAIN_TASK_PRIO,
                 (CPU_STK * )&MAIN_TASK_STK[0],
                 (CPU_STK_SIZE)MAIN_STK_SIZE / 10,
                 (CPU_STK_SIZE)MAIN_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR * )&err);
    printf("1. 创建MAIN任务\r\n");
    //创建SPORT任务
    OSTaskCreate((OS_TCB * )&SPORT_TaskTCB,
                 (CPU_CHAR * )"sport task",
                 (OS_TASK_PTR )sport_task,
                 (void * )0,
                 (OS_PRIO	  )SPORT_TASK_PRIO,
                 (CPU_STK * )&SPORT_TASK_STK[0],
                 (CPU_STK_SIZE)SPORT_STK_SIZE / 10,
                 (CPU_STK_SIZE)SPORT_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR * )&err);
    printf("2. 创建SPORT任务\r\n");
    //创建CAN通信任务
    OSTaskCreate((OS_TCB * )&CAN_TaskTCB,
                 (CPU_CHAR * )"can task",
                 (OS_TASK_PTR )can_task,
                 (void * )0,
                 (OS_PRIO	  )CAN_TASK_PRIO,
                 (CPU_STK * )&CAN_TASK_STK[0],
                 (CPU_STK_SIZE)CAN_STK_SIZE / 10,
                 (CPU_STK_SIZE)CAN_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR * )&err);
    printf("3. 创建CAN通信任务\r\n");
    //创建数据上传任务
    OSTaskCreate((OS_TCB * )&RESPOND_TaskTCB,
                 (CPU_CHAR * )"respond task",
                 (OS_TASK_PTR )respond_task,
                 (void * )0,
                 (OS_PRIO	  )RESPOND_TASK_PRIO,
                 (CPU_STK * )&RESPOND_TASK_STK[0],
                 (CPU_STK_SIZE)RESPOND_STK_SIZE / 10,
                 (CPU_STK_SIZE)RESPOND_STK_SIZE,
                 (OS_MSG_QTY  )RESPOND_TASK_Q_NUM,
                 (OS_TICK	  )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR * )&err);
    printf("4. 创建上位机通信任务\r\n");
    //创建超声波传感器任务
    OSTaskCreate((OS_TCB * )&UNDM_TaskTCB,
                 (CPU_CHAR * )"undm task",
                 (OS_TASK_PTR )undm_task,
                 (void * )0,
                 (OS_PRIO	  )UNDM_TASK_PRIO,
                 (CPU_STK * )&UNDM_TASK_STK[0],
                 (CPU_STK_SIZE)UNDM_STK_SIZE / 10,
                 (CPU_STK_SIZE)UNDM_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void * )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR * )&err);
    printf("5. 创建超声波传感器任务\r\n");
    OSFlagCreate(&UNDM_FLAG_GRP, "undm_flag_grp", 0, &err);
    OS_CRITICAL_ENTER();	//进入临界区
    //创建一个信号量
    OSSemCreate ((OS_SEM *	)&CAN_SEM,
                 (CPU_CHAR *)"CAN_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR *	)&err);
    OS_CRITICAL_EXIT();	//退出临界区
	printf("删除初始化处理函数\r\n");
    OSTaskDel((OS_TCB *)0, &err);	//删除start_task任务自身

}




