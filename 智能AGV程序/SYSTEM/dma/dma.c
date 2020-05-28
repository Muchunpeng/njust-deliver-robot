#include "dma.h"

//DMA���ú���
void DMA_Config(DMA_Stream_TypeDef* pDMA_Stream,
	uint32_t channelx,
	uint32_t PeripheralAddr,
	uint32_t MemoryAddr,
	uint32_t TranDir,
	uint8_t BufferSize)
{
	DMA_InitTypeDef DMAInitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);			//ʹ��DMA1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);			//ʹ��DMA2ʱ��
	
	DMA_DeInit(pDMA_Stream);									//��Ĭ�ϲ�����ʼ��������
	while (DMA_GetCmdStatus(pDMA_Stream)!= DISABLE);			//�ȴ�DMA������ 
	//����DMA������
	DMAInitStructure.DMA_Channel = channelx;  					//ͨ��ѡ��
	DMAInitStructure.DMA_PeripheralBaseAddr = PeripheralAddr;	//DMA�����ַ
	DMAInitStructure.DMA_Memory0BaseAddr = MemoryAddr;		 	//DMA �洢��0��ַ
	DMAInitStructure.DMA_DIR = TranDir;							//�洢��������ģʽ
	DMAInitStructure.DMA_BufferSize = BufferSize;				//��������С 
	DMAInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//���������ģʽ
	DMAInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			//�洢������ģʽ
	DMAInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMAInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//�洢�����ݳ���:8λ
	DMAInitStructure.DMA_Mode = DMA_Mode_Normal;					// ʹ����ͨģʽ 
	DMAInitStructure.DMA_Priority = DMA_Priority_Medium;			//�е����ȼ�
	DMAInitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMAInitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMAInitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//�洢��ͻ�����δ���
	DMAInitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//����ͻ�����δ���
	DMA_Init(pDMA_Stream, &DMAInitStructure);							//��ʼ��DMA1 Stream	
	
}
//DMA1�������ж�ͨ������
void DMAIRQ_Config(
	DMA_Stream_TypeDef*pDMAStream,
	uint8_t DMA_Streamx_IRQn,
	uint8_t PreemptionPriority,
	uint8_t SubPriority)
{		
	NVIC_InitTypeDef NVICInitStructure;
	DMA_ITConfig(pDMAStream,DMA_IT_TC,ENABLE);				//ʹ��DMA����������ж�
	NVICInitStructure.NVIC_IRQChannel=DMA_Streamx_IRQn;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;
	NVICInitStructure.NVIC_IRQChannelSubPriority=SubPriority;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVICInitStructure);
}

//�������ݷ��ͽ��տ�ͨ��DMA��������жϼ��
//���������ݷ��ͽ��������USART�����жϣ��ڼ�⵽�������������ݺ�һ���ֽ�ʱ����û��
//���յ����ݣ����������ж�

