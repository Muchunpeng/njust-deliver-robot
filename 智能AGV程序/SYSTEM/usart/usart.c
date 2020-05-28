#include "system.h"
#include "usart.h"
#include "dma.h"

uint8_t USART1_RxBuffer[12];

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define USART_CMD_LEN			12
#define USART1_DMA_RxBufferSize 12

uint8_t USART1_DMA_RxBuffer[USART1_DMA_RxBufferSize];
uint8_t USART1_RxCnt = 0;


#if DMA_TX_ENABLE
#define USART1_DMA_TxBufferSize 12
uint8_t USART1_DMA_TxBuffer[USART1_DMA_TxBufferSize];
uint8_t USART1_TxCnt = 0;
FlagStatus DMATx_Busy = RESET;
#endif
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40) == 0); //ѭ������,ֱ���������
    USART1->DR = (u8) ch;
    return ch;
}
#endif

//��ʼ��IO ����1
//bound:������
void uart_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ��USART1ʱ��

    //����1��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //GPIOA9����ΪUSART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10����ΪUSART1

    //USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9��PA10

    //USART1 ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;//����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������1

    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); //ʹ��USART1 DMA����
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); //ʹ��USART1 DMA����

    //USART_ClearFlag(USART1, USART_FLAG_TC);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//��������ж�
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

//����1�����ж�
void USART1_IRQHandler(void)
{    static uint8_t USART1_RxBuffer[12] = {0};
    OS_ERR err;
	memset(USART1_RxBuffer, 0, 12);
    OSIntEnter();
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        DMA_Cmd(DMA2_Stream2, DISABLE);
        while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
        USART1_RxCnt = USART1->SR;
        USART1_RxCnt = USART1->DR;														//�ȶ�SR���DR���
        USART1_RxCnt = USART1_DMA_RxBufferSize -
                       DMA_GetCurrDataCounter(DMA2_Stream2);
        DMA_SetCurrDataCounter(DMA2_Stream2, USART1_DMA_RxBufferSize);
        memcpy(USART1_RxBuffer, USART1_DMA_RxBuffer, USART1_RxCnt);
        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);					//���DMA2_Steam2������ɱ�־
        DMA_Cmd(DMA2_Stream2, ENABLE);												//��DMA
        if(USART1_RxBuffer[0])				//�ж��Ƿ�Ϊ��֡
            OSTaskQPost((OS_TCB *	)&RESPOND_TaskTCB,	//������wifi������Ϣ
                        (void *		)USART1_RxBuffer,
                        (OS_MSG_SIZE)USART_CMD_LEN,
                        (OS_OPT		)OS_OPT_POST_FIFO,
                        (OS_ERR *	)&err);
        
    }
    OSIntExit();    	//�˳��ж�
}

void MY_DMA_Init(void)
{
    //����USART1 DMA����
    DMA_Config(DMA2_Stream2,
               DMA_Channel_4,
               (uint32_t)&USART1->DR,
               (uint32_t)USART1_DMA_RxBuffer,
               DMA_DIR_PeripheralToMemory,
               USART1_DMA_RxBufferSize);						//DMA���ã�USART1����

    DMA_Cmd(DMA2_Stream2, DISABLE);     //�ر�DMA
    while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
    DMA_SetCurrDataCounter(DMA2_Stream2, USART1_DMA_RxBufferSize);
    DMA_Cmd(DMA2_Stream2, ENABLE);     	//��DMA
#if DMA_TX_ENABLE
    DMA_Config(DMA2_Stream7,
               DMA_Channel_4,
               (uint32_t)&USART1->DR,
               (uint32_t)USART1_DMA_TxBuffer,
               DMA_DIR_MemoryToPeripheral,
               USART1_DMA_TxBufferSize);
    DMAIRQ_Config(DMA2_Stream7,	DMA2_Stream7_IRQn, 0x03, 0x03);			//����DMA2_Stream7��������ж�
#endif

}

#if DMA_TX_ENABLE
void DMA2_Stream7_IRQHandler()
{
    if(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) != RESET)
    {
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
    }
}
#endif

void AGV_SendMsg(u8 *Data, u8 len)
{

#if DMA_TX_ENABLE
	DMA_Cmd(DMA2_Stream7, DISABLE);                      //�ر�DMA����
while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);	//ȷ��DMA���Ա�����	
    memcpy(USART1_DMA_TxBuffer, Data, len);
    DMA_SetCurrDataCounter(DMA2_Stream7, len);
    DMA_Cmd(DMA2_Stream7, ENABLE);											//ʹ��DMA����
#else
    u32 i = 0;
    for(i = 0; i < len; i++)
    {
        USART_SendData(USART1, Data[i]);
        while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
    }
#endif
}

