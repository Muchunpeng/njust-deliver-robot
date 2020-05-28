#include "system.h"
#include "led.h"


//UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()


//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С
#define START_STK_SIZE 		128
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

#define MAIN_TASK_PRIO		7				//�������ȼ�
#define MAIN_STK_SIZE 		128				//�����ջ��С	
OS_TCB MAIN_TaskTCB;						//������ƿ�
CPU_STK MAIN_TASK_STK[MAIN_STK_SIZE];		//�����ջ
void main_task(void *p_arg);

#define SPORT_TASK_PRIO		4				//�������ȼ�
#define SPORT_STK_SIZE 		128				//�����ջ��-С	
OS_TCB SPORT_TaskTCB;						//������ƿ�
CPU_STK SPORT_TASK_STK[SPORT_STK_SIZE];		//�����ջ
void sport_task(void *p_arg);//������

//CAN����
#define CAN_TASK_PRIO		5				//�������ȼ�
#define CAN_STK_SIZE 		128				//�����ջ��С
OS_TCB CAN_TaskTCB;							//������ƿ�
CPU_STK CAN_TASK_STK[CAN_STK_SIZE];			//�����ջ
void can_task(void *p_arg);

//�����ϴ�����
#define RESPOND_TASK_PRIO	6				//�������ȼ�
#define RESPOND_STK_SIZE 	128				//�����ջ��С
#define RESPOND_TASK_Q_NUM		1			//�������ڽ���Ϣ���еĳ���
OS_TCB RESPOND_TaskTCB;						//������ƿ�
CPU_STK RESPOND_TASK_STK[RESPOND_STK_SIZE];	//�����ջ
void respond_task(void *p_arg);

#define UNDM_TASK_PRIO		10
#define UNDM_STK_SIZE		256
OS_TCB 	UNDM_TaskTCB;
CPU_STK UNDM_TASK_STK[UNDM_STK_SIZE];
void undm_task(void *p_arg);

OS_FLAG_GRP UNDM_FLAG_GRP;
OS_SEM	CAN_SEM;		//���ڵȴ�CAN������Ϣ

//������
int main(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    My_Init();
    OSInit(&err);		    //��ʼ��UCOSIII
    OS_CRITICAL_ENTER();	//�����ٽ���
    //������ʼ����
    OSTaskCreate((OS_TCB * )&StartTaskTCB,		//������ƿ�
                 (CPU_CHAR * )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void * )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE / 10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void * )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR * )&err);				//��Ÿú�������ʱ�ķ���ֵ
    OS_CRITICAL_EXIT();	//�˳��ٽ���
    OSStart(&err);      //����UCOSIII
}

//��ʼ����������
void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);  	//ͳ������
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
    //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
    OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif


    //����MAIN����
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
    printf("1. ����MAIN����\r\n");
    //����SPORT����
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
    printf("2. ����SPORT����\r\n");
    //����CANͨ������
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
    printf("3. ����CANͨ������\r\n");
    //���������ϴ�����
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
    printf("4. ������λ��ͨ������\r\n");
    //��������������������
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
    printf("5. ��������������������\r\n");
    OSFlagCreate(&UNDM_FLAG_GRP, "undm_flag_grp", 0, &err);
    OS_CRITICAL_ENTER();	//�����ٽ���
    //����һ���ź���
    OSSemCreate ((OS_SEM *	)&CAN_SEM,
                 (CPU_CHAR *)"CAN_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR *	)&err);
    OS_CRITICAL_EXIT();	//�˳��ٽ���
	printf("ɾ����ʼ��������\r\n");
    OSTaskDel((OS_TCB *)0, &err);	//ɾ��start_task��������

}




