/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	usart.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-02-27
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		��Ƭ�����������ʼ������ʽ����ӡ
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��������
#include "my_mcu_usart.h"
#include "my_mcu_nvic.h"
#include "my_mcu_dma.h"

//Ӳ������
#include "my_usart.h"
#include "my_delay.h"

//C��
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


ALTER_INFO alter_info;


#if(USART_DMA_TX_EN == 1)
unsigned char UsartPrintfBuf[128];
#endif


/*
************************************************************
*	�������ƣ�	Usart1_Init
*
*	�������ܣ�	����1��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PA9		RX-PA10
*				���ͣ�DMA1_Channel4
*				���գ�DMA1_Channel5
*				δʹ��DMA_TC�жϣ�������RTOS�汾���п��ܻ�ر����ж϶�������ѭ��
************************************************************
*/
void Usart1_Init(unsigned int baud)
{
	
	MCU_USART_Init(USART1, baud, USART_HardwareFlowControl_None, USART_Mode_Rx | USART_Mode_Tx,
					USART_Parity_No, USART_StopBits_1, USART_WordLength_8b);
	
	USART_Cmd(USART1, ENABLE);														//ʹ�ܴ���
	
#if(USART_DMA_RX_EN == 0)
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
#endif

	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);									//ʹ��IDLE�ж�
	
	MCU_NVIC_Init(USART1_IRQn, ENABLE, 1, 2);
	
#if(USART_DMA_TX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel4, (unsigned int)&USART1->DR, NULL, DMA_DIR_PeripheralDST, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);									//ʹ��USART1��DMA���͹���
	
	USARTx_ResetMemoryBaseAddr(USART1, (unsigned int)UsartPrintfBuf, 1, USART_TX_TYPE);//����һ������
#endif

#if(USART_DMA_RX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel5, (unsigned int)&USART1->DR, NULL, DMA_DIR_PeripheralSRC, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);									//ʹ��USART1��DMA���չ���
#endif

}

/*
************************************************************
*	�������ƣ�	Usart2_Init
*
*	�������ܣ�	����2��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PA2		RX-PA3
*				���ͣ�DMA1_Channel7
*				���գ�DMA1_Channel6
*				δʹ��DMA_TC�жϣ�������RTOS�汾���п��ܻ�ر����ж϶�������ѭ��
************************************************************
*/
void Usart2_Init(unsigned int baud)
{
	
	MCU_USART_Init(USART2, baud, USART_HardwareFlowControl_None, USART_Mode_Rx | USART_Mode_Tx,
					USART_Parity_No, USART_StopBits_1, USART_WordLength_8b);
	
	USART_Cmd(USART2, ENABLE);														//ʹ�ܴ���

#if(USART_DMA_RX_EN == 0)
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
#endif

	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);									//ʹ��IDLE�ж�
	
	MCU_NVIC_Init(USART2_IRQn, ENABLE, 0, 0);
	
#if(USART_DMA_TX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel7, (unsigned int)&USART2->DR, NULL, DMA_DIR_PeripheralDST, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);									//ʹ��USART2��DMA���͹���
	
	USARTx_ResetMemoryBaseAddr(USART2, (unsigned int)UsartPrintfBuf, 1, USART_TX_TYPE);//����һ������
#endif
	
#if(USART_DMA_RX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel6, (unsigned int)&USART2->DR, NULL, DMA_DIR_PeripheralSRC, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	MCU_NVIC_Init(DMA1_Channel6_IRQn, ENABLE, 1, 2);
	
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);									//������������ж�

	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);									//ʹ��USART2��DMA���չ���
#endif

}

/*
************************************************************
*	�������ƣ�	USARTx_ResetMemoryBaseAddr
*
*	�������ܣ�	����DMA�ڴ��ַ��ʹ�ܷ���
*
*	��ڲ�����	USARTx��������
*				mAddr���ڴ��ֵַ
*				num�����η��͵����ݳ���(�ֽ�)
*
*	���ز�����	��
*
*	˵����		
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
*	�������ƣ�	Usart_SendString
*
*	�������ܣ�	�������ݷ���
*
*	��ڲ�����	USARTx��������
*				str��Ҫ���͵�����
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

#if(USART_DMA_TX_EN == 0)
	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//��������
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
	}
#else
	unsigned int mAddr = (unsigned int)str;
	
	if(USARTx == USART1)
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);					//�ȴ�ͨ��4�������
		DMA_ClearFlag(DMA1_FLAG_TC4);										//���ͨ��4������ɱ�־
	}
	else if(USARTx == USART2)
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET);					//�ȴ�ͨ��7�������
		DMA_ClearFlag(DMA1_FLAG_TC7);										//���ͨ��7������ɱ�־
	}
	
	USARTx_ResetMemoryBaseAddr(USARTx, mAddr, len, USART_TX_TYPE);
#endif

}

/*
************************************************************
*	�������ƣ�	UsartPrintf
*
*	�������ܣ�	��ʽ����ӡ
*
*	��ڲ�����	USARTx��������
*				fmt����������
*
*	���ز�����	��
*
*	˵����		
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
		while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);					//�ȴ�ͨ��4�������
		DMA_ClearFlag(DMA1_FLAG_TC4);										//���ͨ��4������ɱ�־
	}
	else if(USARTx == USART2)
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET);					//�ȴ�ͨ��7�������
		DMA_ClearFlag(DMA1_FLAG_TC7);										//���ͨ��7������ɱ�־
	}
	
	memset(UsartPrintfBuf, 0, sizeof(UsartPrintfBuf));					//���buffer
#endif
	
	va_start(ap, fmt);
	vsprintf((char *)UsartPrintfBuf, fmt, ap);							//��ʽ��
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
*	�������ƣ�	USART1_IRQHandler
*
*	�������ܣ�	����1�շ��ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void USART1_IRQHandler(void)
{
	
	RTOS_EnterInt();

#if(USART_DMA_RX_EN == 0)
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)		//�����ж�
	{
		alter_info.alter_count %= sizeof(alter_info.alter_buf);		//��ֹ���ڱ�ˢ��
		
        alter_info.alter_buf[alter_info.alter_count++] = USART1->DR;
		
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
#endif

	if(USART_GetFlagStatus(USART1, USART_FLAG_IDLE) != RESET)
	{
		//���Ϳ����ж�֮�󣬶�ȡ���ݷ���master
		my_drv_uart_send_mastter(alter_info.alter_buf,alter_info.alter_count);
		alter_info.rev_idle = 1;
		alter_info.alter_count = 0;
		memset(alter_info.alter_buf,0,1024);
		
		USART1->DR;													//��ȡ����ע�⣺������Ҫ�������ܹ�����жϱ�־λ
		USART_ClearFlag(USART1, USART_IT_IDLE);
		
#if(USART_DMA_RX_EN == 1)
		DMA_Cmd(DMA1_Channel5, DISABLE);
		
		DMA1_Channel5->CNDTR = sizeof(alter_info.alter_buf);		//���������´ν��յĳ��ȣ������޷������´�DMA����
		
		DMA_Cmd(DMA1_Channel5, ENABLE);
#endif	

	}
	RTOS_ExitInt();
}


