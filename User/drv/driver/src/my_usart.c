/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	usart.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-02-27
	*
	*	版本： 		V1.0
	*
	*	说明： 		单片机串口外设初始化，格式化打印
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机相关组件
#include "my_mcu_usart.h"
#include "my_mcu_nvic.h"
#include "my_mcu_dma.h"

//硬件驱动
#include "my_usart.h"
#include "my_delay.h"

//C库
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


ALTER_INFO alter_info;


#if(USART_DMA_TX_EN == 1)
unsigned char UsartPrintfBuf[128];
#endif


/*
************************************************************
*	函数名称：	Usart1_Init
*
*	函数功能：	串口1初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PA9		RX-PA10
*				发送：DMA1_Channel4
*				接收：DMA1_Channel5
*				未使用DMA_TC中断，发现在RTOS版本中有可能会关闭总中断而导致死循环
************************************************************
*/
void Usart1_Init(unsigned int baud)
{
	
	MCU_USART_Init(USART1, baud, USART_HardwareFlowControl_None, USART_Mode_Rx | USART_Mode_Tx,
					USART_Parity_No, USART_StopBits_1, USART_WordLength_8b);
	
	USART_Cmd(USART1, ENABLE);														//使能串口
	
#if(USART_DMA_RX_EN == 0)
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//使能接收中断
#endif

	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);									//使能IDLE中断
	
	MCU_NVIC_Init(USART1_IRQn, ENABLE, 1, 2);
	
#if(USART_DMA_TX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel4, (unsigned int)&USART1->DR, NULL, DMA_DIR_PeripheralDST, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);									//使能USART1的DMA发送功能
	
	USARTx_ResetMemoryBaseAddr(USART1, (unsigned int)UsartPrintfBuf, 1, USART_TX_TYPE);//发送一次数据
#endif

#if(USART_DMA_RX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel5, (unsigned int)&USART1->DR, NULL, DMA_DIR_PeripheralSRC, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);									//使能USART1的DMA接收功能
#endif

}

/*
************************************************************
*	函数名称：	Usart2_Init
*
*	函数功能：	串口2初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PA2		RX-PA3
*				发送：DMA1_Channel7
*				接收：DMA1_Channel6
*				未使用DMA_TC中断，发现在RTOS版本中有可能会关闭总中断而导致死循环
************************************************************
*/
void Usart2_Init(unsigned int baud)
{
	
	MCU_USART_Init(USART2, baud, USART_HardwareFlowControl_None, USART_Mode_Rx | USART_Mode_Tx,
					USART_Parity_No, USART_StopBits_1, USART_WordLength_8b);
	
	USART_Cmd(USART2, ENABLE);														//使能串口

#if(USART_DMA_RX_EN == 0)
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//使能接收中断
#endif

	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);									//使能IDLE中断
	
	MCU_NVIC_Init(USART2_IRQn, ENABLE, 0, 0);
	
#if(USART_DMA_TX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel7, (unsigned int)&USART2->DR, NULL, DMA_DIR_PeripheralDST, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);									//使能USART2的DMA发送功能
	
	USARTx_ResetMemoryBaseAddr(USART2, (unsigned int)UsartPrintfBuf, 1, USART_TX_TYPE);//发送一次数据
#endif
	
#if(USART_DMA_RX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel6, (unsigned int)&USART2->DR, NULL, DMA_DIR_PeripheralSRC, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	MCU_NVIC_Init(DMA1_Channel6_IRQn, ENABLE, 1, 2);
	
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);									//开启接收完成中断

	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);									//使能USART2的DMA接收功能
#endif

}

/*
************************************************************
*	函数名称：	USARTx_ResetMemoryBaseAddr
*
*	函数功能：	重设DMA内存地址并使能发送
*
*	入口参数：	USARTx：串口组
*				mAddr：内存地址值
*				num：本次发送的数据长度(字节)
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void USARTx_ResetMemoryBaseAddr(USART_TypeDef *USARTx, unsigned int mAddr, unsigned short num, _Bool type)
{

#if(USART_DMA_TX_EN == 1)
	if(type == USART_TX_TYPE)
	{
		if(USARTx == USART1)
			MCU_DMA_ResetMemoryBaseAddr(DMA1_Channel4, mAddr, num);
		else if(USARTx == USART2)
			MCU_DMA_ResetMemoryBaseAddr(DMA1_Channel7, mAddr, num);
	}
#endif
	
#if(USART_DMA_RX_EN == 1)
	if(type == USART_RX_TYPE)
	{
		if(USARTx == USART1)
			MCU_DMA_ResetMemoryBaseAddr(DMA1_Channel5, mAddr, num);
		else if(USARTx == USART2)
			MCU_DMA_ResetMemoryBaseAddr(DMA1_Channel6, mAddr, num);
	}
#endif

}

/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

#if(USART_DMA_TX_EN == 0)
	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}
#else
	unsigned int mAddr = (unsigned int)str;
	
	if(USARTx == USART1)
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);					//等待通道4传输完成
		DMA_ClearFlag(DMA1_FLAG_TC4);										//清除通道4传输完成标志
	}
	else if(USARTx == USART2)
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET);					//等待通道7传输完成
		DMA_ClearFlag(DMA1_FLAG_TC7);										//清除通道7传输完成标志
	}
	
	USARTx_ResetMemoryBaseAddr(USARTx, mAddr, len, USART_TX_TYPE);
#endif

}

/*
************************************************************
*	函数名称：	UsartPrintf
*
*	函数功能：	格式化打印
*
*	入口参数：	USARTx：串口组
*				fmt：不定长参
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{

	va_list ap;
	
#if(USART_DMA_TX_EN == 0)
	unsigned char UsartPrintfBuf[128];
#endif
	
	unsigned char *pStr = UsartPrintfBuf;
	
#if(USART_DMA_TX_EN == 1)
	if(USARTx == USART1)
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);					//等待通道4传输完成
		DMA_ClearFlag(DMA1_FLAG_TC4);										//清除通道4传输完成标志
	}
	else if(USARTx == USART2)
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET);					//等待通道7传输完成
		DMA_ClearFlag(DMA1_FLAG_TC7);										//清除通道7传输完成标志
	}
	
	memset(UsartPrintfBuf, 0, sizeof(UsartPrintfBuf));					//清空buffer
#endif
	
	va_start(ap, fmt);
	vsprintf((char *)UsartPrintfBuf, fmt, ap);							//格式化
	va_end(ap);
	
#if(USART_DMA_TX_EN == 1)
	USARTx_ResetMemoryBaseAddr(USARTx, (unsigned int)UsartPrintfBuf,
							strlen((const char *)pStr), USART_TX_TYPE);
#else
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
#endif

}

/*
************************************************************
*	函数名称：	USART1_IRQHandler
*
*	函数功能：	串口1收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void USART1_IRQHandler(void)
{
	
	RTOS_EnterInt();

#if(USART_DMA_RX_EN == 0)
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)		//接收中断
	{
		alter_info.alter_count %= sizeof(alter_info.alter_buf);		//防止串口被刷爆
		
        alter_info.alter_buf[alter_info.alter_count++] = USART1->DR;
		
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
#endif

	if(USART_GetFlagStatus(USART1, USART_FLAG_IDLE) != RESET)
	{
		//发送空闲中断之后，读取数据发给master
		my_drv_uart_send_mastter(alter_info.alter_buf,alter_info.alter_count);
		alter_info.rev_idle = 1;
		alter_info.alter_count = 0;
		memset(alter_info.alter_buf,0,1024);
		
		USART1->DR;													//读取数据注意：这句必须要，否则不能够清除中断标志位
		USART_ClearFlag(USART1, USART_IT_IDLE);
		
#if(USART_DMA_RX_EN == 1)
		DMA_Cmd(DMA1_Channel5, DISABLE);
		
		DMA1_Channel5->CNDTR = sizeof(alter_info.alter_buf);		//重新设置下次接收的长度，否则无法启动下次DMA接收
		
		DMA_Cmd(DMA1_Channel5, ENABLE);
#endif	

	}
	RTOS_ExitInt();
}


