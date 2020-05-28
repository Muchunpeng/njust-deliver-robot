#include "system.h"
#include "can.h"
#include "iwdg.h"
CanRxMsg RxMessage;

//�жϷ�����
void CAN1_RX0_IRQHandler(void)
{
    OSIntEnter();
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
    CAN_Check(&RxMessage, &my_agv);
    OSIntExit();    	//�˳��ж�
}

/**************************************************************************
�������ܣ��������ļ��
��ڲ���������
����  ֵ���ɹ���1
		��ʧ�ܣ�0
��ע��
**************************************************************************/
u16 CAN_Check(CanRxMsg *rxmsg, AGV_MODEL *agv)
{
    OS_ERR err;
    u16 cob = rxmsg->StdId & 0xFF0;
    switch (cob)
    {
    case (TPDO2):
        PDO_Back(&RxMessage, &agv->right_wheel);
        PDO_Back(&RxMessage, &agv->left_wheel);
        break;
    case (TPDO3):
        PDO_Back(&RxMessage, &agv->right_wheel);
        PDO_Back(&RxMessage, &agv->left_wheel);
        break;
    case (SDO_READ):
        OSSemPost(&CAN_SEM, OS_OPT_POST_1, &err); //�����ź���
        break;
    case (Heart_Beat):
        if(rxmsg->Data[0] == 0x05)
            IWDG_Feed();//ι��
        if((rxmsg->StdId & 0x00F) == 1)
            agv->left_wheel.nmt = rxmsg->Data[0];
        else if((rxmsg->StdId & 0x00F) == 2)
            agv->right_wheel.nmt = rxmsg->Data[0];
        break;
    default:
        return 0;
    }
    return cob;
}

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)
//len:���ݳ���(���Ϊ8)
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u16 id, u8 *msg, u8 len)
{
    u8 mbox;
    u16	i;
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    CanTxMsg TxMessage;
    TxMessage.StdId = id;	 // ��׼��ʶ��Ϊ0
    TxMessage.ExtId = 0;	 // ������չ��ʾ����29λ��
    TxMessage.IDE = 0;		 // ʹ����չ��ʶ��
    TxMessage.RTR = 0;		 // ��Ϣ����Ϊ����֡��һ֡8λ
    TxMessage.DLC = len;							 // ������֡��Ϣ
    memcpy(TxMessage.Data, msg, len);
    mbox = CAN_Transmit(CAN1, &TxMessage);
    while((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0x8000))i++;	//�ȴ����ͽ���
    CPU_CRITICAL_EXIT();
    if(i >= 0x8000)return 1;
    return 0;

}

//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ42M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//������Ϊ:42M/((6+7+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��;

u8 CAN1_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    //ʹ�����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��
    //��ʼ��GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
    //���Ÿ���ӳ������
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); //GPIOA11����ΪCAN1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); //GPIOA12����ΪCAN1
    //CAN��Ԫ����
    CAN_InitStructure.CAN_TTCM = DISABLE;	//��ʱ�䴥��ͨ��ģʽ
    CAN_InitStructure.CAN_ABOM = DISABLE;	//����Զ����߹���
    CAN_InitStructure.CAN_AWUM = DISABLE; //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN_InitStructure.CAN_NART = ENABLE;	//��ֹ�����Զ�����
    CAN_InitStructure.CAN_RFLM = DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�
    CAN_InitStructure.CAN_TXFP = DISABLE;	//���ȼ��ɱ��ı�ʶ������
    CAN_InitStructure.CAN_Mode = mode;	 //ģʽ����
    CAN_InitStructure.CAN_SJW = tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
    CAN_InitStructure.CAN_BS1 = tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2 = tbs2; //Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
    CAN_InitStructure.CAN_Prescaler = brp; //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1

    //���ù�����
    CAN_FilterInitStructure.CAN_FilterNumber = 0;	 //������0
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; //32λ
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000; ////32λID
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //32λMASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //������0������FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //���������0
    CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //FIFO0��Ϣ�Һ��ж�����.

    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    return 0;
}











