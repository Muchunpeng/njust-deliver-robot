#ifndef __RS232_H
#define __RS232_H
#include "usart.h"
#include "dma.h"
#include "led.h"
#define HOST_DMA_TX_EN	1
#define USE_CHKS 	1

#define ErrID 		0x01
#define ErrIndex 	0x02
#define ErrSubCode 	0x03
#define ErrCHKS		0x04
#define ErrTimeout	0x05
#define RightRes	0x12
#define RS232_ERR_RET 0x00FFFFFF

#define USART1_BUFFER_SIZE	48
#define USART1_DMA_RxBufferSize USART1_BUFFER_SIZE

//****************�ŷ�����������Ϣ�ṹ*************
/*
 * ID:������ID,���������ŷ�������ID�ֱ�����Ϊ1��2
 * Code:������;23�����ĸ��ֽڵ�����Data[0..3];2b-���������ֽڵ�����Data[0..1];2f-����һ���ֽڵ�����Data[0]
 * IndexLByte���������ֽ�
 * IndexHbyte:�������ֽ�
 * SubIndex:������
 * Data[4]:Ҫд��Ŀ����������
 * CHKS:У����
 * */

//typedef struct{
//	uint16_t ID:8;
//	uint16_t Code:8;
//	uint16_t Index;
//	uint16_t SubIndex:8;
//	uint16_t Data0:8;
//	uint16_t Data1:8;
//	uint16_t Data2:8;
//	uint16_t Data3:8;
//	uint16_t CHKS:8;
//}_DriverAddrFormat;

extern uint8_t USART1_RxCnt;
extern uint8_t USART1_RxBuffer[USART1_BUFFER_SIZE];
extern uint8_t USART1_DMA_RxBuffer[USART1_DMA_RxBufferSize];

int GenerateCHKS(uint8_t*pData,uint8_t DataLen);
void RS232_SendMsg(uint8_t*pMsg,uint8_t dataLen);

void RS232Init(void);
void USART_SendMsg(USART_TypeDef*USARTx,uint8_t*pMsg,uint8_t dataLen);

uint8_t WaitForRes(uint8_t*pMsgReq,int32_t Timeout);
uint32_t Rs232Write(uint32_t CANOpenAddr,uint8_t ObjID,uint32_t* Data);
uint32_t Rs232Read(uint32_t CANOpenAddr,uint8_t ObjID);

#endif

