#ifndef __DMA_H
#define __DMA_H
#include <system.h>
void DMA_Config(
	DMA_Stream_TypeDef* pDMA_Stream,	//������
	uint32_t channelx,								//DMAͨ��
	uint32_t PeripheralAddr,					//�����ַ
	uint32_t MemoryAddr,							//�ڴ��ַ
	uint32_t TranDir,									//���䷽��
	uint8_t BufferSize);							//����������

void DMAIRQ_Config(
	DMA_Stream_TypeDef*pDMAStream,
	uint8_t DMA_Streamx_IRQn,
	uint8_t PreemptionPriority,
	uint8_t SubPriority);


//DMA1ͨ��ӳ��
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
ʹ��DMA�����õ���Ҫ������
1��Դ��ַ��Ŀ�ĵ�ַ��2���������ݸ�����3����������ж�(��ѡ)��4���䷽��
*/


#endif




