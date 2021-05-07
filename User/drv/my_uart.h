/*******************************************************

* �ļ�����my_uart.h

* ���ߣ�  luyangguang

* ������ ����������ʼ��

* �޸��ˣ���¬���⡵

* �޸�ʱ�䣺2021-04-24

* �޸����ݣ�����
*******************************************************/

/*******************  ͷ�ļ����� ***********************/
/******************************************************/

#ifndef __MY_UART_H_
#define	__MY_UART_H_


#include "stm32f10x.h"
#include "my_com.h" 

#include <stdio.h>

/******************************************************/
/*******************  ȫ�ֱ������� *********************/
/******************************************************/

/******************************************************/
/*******************  �������Ͷ��� *********************/
/******************************************************/

/******************************************************/
/*******************  �ⲿ�������� *********************/
/******************************************************/

/******************************************************/
/*******************  ���غ������� *********************/
/******************************************************/

/******************************************************/
/*******************  �ⲿ�������� *********************/
/******************************************************/



/** 
  * ���ں궨�壬��ͬ�Ĵ��ڹ��ص����ߺ�IO��һ������ֲʱ��Ҫ�޸��⼸����
	* 1-�޸�����ʱ�ӵĺ꣬uart1���ص�apb2���ߣ�����uart���ص�apb1����
	* 2-�޸�GPIO�ĺ�
  */
	
// ����1-USART1
#define  pal_USARTx                   USART1
#define  pal_USART_CLK                RCC_APB2Periph_USART1
#define  pal_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  pal_USART_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  pal_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  pal_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  pal_USART_TX_GPIO_PORT       GPIOA   
#define  pal_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  pal_USART_RX_GPIO_PORT       GPIOA
#define  pal_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  pal_USART_IRQ                USART1_IRQn
#define  pal_USART_IRQHandler         USART1_IRQHandler


// ����2-USART2
//#define  pal_USARTx                   USART2
//#define  pal_USART_CLK                RCC_APB1Periph_USART2
//#define  pal_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  pal_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  pal_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  pal_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  pal_USART_TX_GPIO_PORT       GPIOA   
//#define  pal_USART_TX_GPIO_PIN        GPIO_Pin_2
//#define  pal_USART_RX_GPIO_PORT       GPIOA
//#define  pal_USART_RX_GPIO_PIN        GPIO_Pin_3

//#define  pal_USART_IRQ                USART2_IRQn
//#define  pal_USART_IRQHandler         USART2_IRQHandler

// ����3-USART3
//#define  pal_USARTx                   USART3
//#define  pal_USART_CLK                RCC_APB1Periph_USART3
//#define  pal_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  pal_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  pal_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
//#define  pal_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  pal_USART_TX_GPIO_PORT       GPIOB   
//#define  pal_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  pal_USART_RX_GPIO_PORT       GPIOB
//#define  pal_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  pal_USART_IRQ                USART3_IRQn
//#define  pal_USART_IRQHandler         USART3_IRQHandler

// ����4-UART4
//#define  pal_USARTx                   UART4
//#define  pal_USART_CLK                RCC_APB1Periph_UART4
//#define  pal_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  pal_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  pal_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
//#define  pal_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  pal_USART_TX_GPIO_PORT       GPIOC   
//#define  pal_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  pal_USART_RX_GPIO_PORT       GPIOC
//#define  pal_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  pal_USART_IRQ                UART4_IRQn
//#define  pal_USART_IRQHandler         UART4_IRQHandler


// ����5-UART5
//#define  pal_USARTx                   UART5
//#define  pal_USART_CLK                RCC_APB1Periph_UART5
//#define  pal_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  pal_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  pal_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
//#define  pal_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  pal_USART_TX_GPIO_PORT       GPIOC   
//#define  pal_USART_TX_GPIO_PIN        GPIO_Pin_12
//#define  pal_USART_RX_GPIO_PORT       GPIOD
//#define  pal_USART_RX_GPIO_PIN        GPIO_Pin_2

//#define  pal_USART_IRQ                UART5_IRQn
//#define  pal_USART_IRQHandler         UART5_IRQHandler


void USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);






















#endif /* __MY_UART_H_ */

