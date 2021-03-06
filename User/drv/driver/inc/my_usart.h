#ifndef _MY_USART_H_
#define _MY_USART_H_


#include "stm32f10x.h"


#define USART_DEBUG		USART1		//调试打印所使用的串口组


//DMA暂时无法开启，有问题，后面调整
#define USART_DMA_TX_EN		0		//1-使能DMA发送		0-失能DMA发送

#define USART_DMA_RX_EN		0		//1-使能DMA接收		0-失能DMA接收


#define USART_TX_TYPE		1

#define USART_RX_TYPE		1


typedef struct
{

	char alter_buf[1024];
	unsigned int alter_count;
	_Bool rev_idle;

} ALTER_INFO;

extern ALTER_INFO alter_info;


void Usart1_Init(unsigned int baud);

void Usart2_Init(unsigned int baud);

void USARTx_ResetMemoryBaseAddr(USART_TypeDef *USARTx, unsigned int mAddr, unsigned short num, _Bool type);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);


#endif
