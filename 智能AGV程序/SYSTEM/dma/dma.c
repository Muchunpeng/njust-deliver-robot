#include "dma.h"

//DMA配置函数
void DMA_Config(DMA_Stream_TypeDef* pDMA_Stream,
	uint32_t channelx,
	uint32_t PeripheralAddr,
	uint32_t MemoryAddr,
	uint32_t TranDir,
	uint8_t BufferSize)
{
	DMA_InitTypeDef DMAInitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);			//使能DMA1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);			//使能DMA2时钟
	
	DMA_DeInit(pDMA_Stream);									//按默认参数初始化数据流
	while (DMA_GetCmdStatus(pDMA_Stream)!= DISABLE);			//等待DMA可配置 
	//配置DMA数据流
	DMAInitStructure.DMA_Channel = channelx;  					//通道选择
	DMAInitStructure.DMA_PeripheralBaseAddr = PeripheralAddr;	//DMA外设地址
	DMAInitStructure.DMA_Memory0BaseAddr = MemoryAddr;		 	//DMA 存储器0地址
	DMAInitStructure.DMA_DIR = TranDir;							//存储器到外设模式
	DMAInitStructure.DMA_BufferSize = BufferSize;				//缓冲区大小 
	DMAInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设非增量模式
	DMAInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			//存储器增量模式
	DMAInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMAInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//存储器数据长度:8位
	DMAInitStructure.DMA_Mode = DMA_Mode_Normal;					// 使用普通模式 
	DMAInitStructure.DMA_Priority = DMA_Priority_Medium;			//中等优先级
	DMAInitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMAInitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMAInitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//存储器突发单次传输
	DMAInitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//外设突发单次传输
	DMA_Init(pDMA_Stream, &DMAInitStructure);							//初始化DMA1 Stream	
	
}
//DMA1数据流中断通道配置
void DMAIRQ_Config(
	DMA_Stream_TypeDef*pDMAStream,
	uint8_t DMA_Streamx_IRQn,
	uint8_t PreemptionPriority,
	uint8_t SubPriority)
{		
	NVIC_InitTypeDef NVICInitStructure;
	DMA_ITConfig(pDMAStream,DMA_IT_TC,ENABLE);				//使能DMA流传输完成中断
	NVICInitStructure.NVIC_IRQChannel=DMA_Streamx_IRQn;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;
	NVICInitStructure.NVIC_IRQChannelSubPriority=SubPriority;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVICInitStructure);
}

//定长数据发送接收可通过DMA传输完成中断检测
//不定长数据发送接收需配合USART空闲中断：在检测到数据总线上数据后，一个字节时间内没有
//接收到数据，触发空闲中断

