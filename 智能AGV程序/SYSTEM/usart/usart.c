#include "system.h"
#include "usart.h"
#include "dma.h"

uint8_t USART1_RxBuffer[12];

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
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
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
    USART1->DR = (u8) ch;
    return ch;
}
#endif

//初始化IO 串口1
//bound:波特率
void uart_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1时钟

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10复用为USART1

    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9，PA10

    //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口1

    USART_Cmd(USART1, ENABLE);  //使能串口1

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); //使能USART1 DMA接收
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); //使能USART1 DMA发送

    //USART_ClearFlag(USART1, USART_FLAG_TC);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启相关中断
    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

//串口1空闲中断
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
        USART1_RxCnt = USART1->DR;														//先读SR后读DR清除
        USART1_RxCnt = USART1_DMA_RxBufferSize -
                       DMA_GetCurrDataCounter(DMA2_Stream2);
        DMA_SetCurrDataCounter(DMA2_Stream2, USART1_DMA_RxBufferSize);
        memcpy(USART1_RxBuffer, USART1_DMA_RxBuffer, USART1_RxCnt);
        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);					//清除DMA2_Steam2传输完成标志
        DMA_Cmd(DMA2_Stream2, ENABLE);												//打开DMA
        if(USART1_RxBuffer[0])				//判断是否为空帧
            OSTaskQPost((OS_TCB *	)&RESPOND_TaskTCB,	//向任务wifi发送消息
                        (void *		)USART1_RxBuffer,
                        (OS_MSG_SIZE)USART_CMD_LEN,
                        (OS_OPT		)OS_OPT_POST_FIFO,
                        (OS_ERR *	)&err);
        
    }
    OSIntExit();    	//退出中断
}

void MY_DMA_Init(void)
{
    //配置USART1 DMA接收
    DMA_Config(DMA2_Stream2,
               DMA_Channel_4,
               (uint32_t)&USART1->DR,
               (uint32_t)USART1_DMA_RxBuffer,
               DMA_DIR_PeripheralToMemory,
               USART1_DMA_RxBufferSize);						//DMA配置，USART1接收

    DMA_Cmd(DMA2_Stream2, DISABLE);     //关闭DMA
    while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
    DMA_SetCurrDataCounter(DMA2_Stream2, USART1_DMA_RxBufferSize);
    DMA_Cmd(DMA2_Stream2, ENABLE);     	//打开DMA
#if DMA_TX_ENABLE
    DMA_Config(DMA2_Stream7,
               DMA_Channel_4,
               (uint32_t)&USART1->DR,
               (uint32_t)USART1_DMA_TxBuffer,
               DMA_DIR_MemoryToPeripheral,
               USART1_DMA_TxBufferSize);
    DMAIRQ_Config(DMA2_Stream7,	DMA2_Stream7_IRQn, 0x03, 0x03);			//配置DMA2_Stream7传输完成中断
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
	DMA_Cmd(DMA2_Stream7, DISABLE);                      //关闭DMA传输
while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);	//确保DMA可以被设置	
    memcpy(USART1_DMA_TxBuffer, Data, len);
    DMA_SetCurrDataCounter(DMA2_Stream7, len);
    DMA_Cmd(DMA2_Stream7, ENABLE);											//使能DMA传输
#else
    u32 i = 0;
    for(i = 0; i < len; i++)
    {
        USART_SendData(USART1, Data[i]);
        while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
    }
#endif
}

