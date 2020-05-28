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

//****************伺服驱动器的消息结构*************
/*
 * ID:驱动器ID,左右两个伺服驱动器ID分别配置为1和2
 * Code:功能码;23发送四个字节的数据Data[0..3];2b-发送两个字节的数据Data[0..1];2f-发送一个字节的数据Data[0]
 * IndexLByte：索引低字节
 * IndexHbyte:索引高字节
 * SubIndex:子索引
 * Data[4]:要写入目标对象的数据
 * CHKS:校验码
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

