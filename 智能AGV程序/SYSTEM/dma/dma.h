#ifndef __DMA_H
#define __DMA_H
#include <system.h>
void DMA_Config(
	DMA_Stream_TypeDef* pDMA_Stream,	//数据流
	uint32_t channelx,								//DMA通道
	uint32_t PeripheralAddr,					//外设地址
	uint32_t MemoryAddr,							//内存地址
	uint32_t TranDir,									//传输方向
	uint8_t BufferSize);							//传输数据量

void DMAIRQ_Config(
	DMA_Stream_TypeDef*pDMAStream,
	uint8_t DMA_Streamx_IRQn,
	uint8_t PreemptionPriority,
	uint8_t SubPriority);


//DMA1通道映射
/*
DMA1_Stream0_Channel_4 USART5_Rx
DMA1_Stream1_Channel_4 USART3_Rx
DMA1_Stream2_Channel_4 USART4_Rx
DMA1_Stream3_Channel_4 USART3_Tx
DMA1_Stream4_Channel_4 USART4_Tx
DMA1_Stream5_Channel_4 USART2_Rx
DMA1_Stream6_Channel_4 USART2_Tx
DMA1_Stream7_Channel_4 USART5_Tx
*/
/*
使用DMA需配置的重要参数：
1、源地址和目的地址；2、传输数据个数；3、传输完成中断(可选)；4传输方向
*/


#endif




