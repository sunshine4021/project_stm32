/*******************************************************

* 文件名：my_uart.h

* 作者：  luyangguang

* 描述： 串口驱动初始化

* 修改人：〈卢阳光〉

* 修改时间：2021-04-24

* 修改内容：创建
*******************************************************/

/*******************  头文件包含 ***********************/
/******************************************************/

#ifndef __MY_UART_H_
#define	__MY_UART_H_


#include "stm32f10x.h"
#include "my_com.h" 

#include <stdio.h>

/******************************************************/
/*******************  全局变量定义 *********************/
/******************************************************/

/******************************************************/
/*******************  数据类型定义 *********************/
/******************************************************/

/******************************************************/
/*******************  外部变量引用 *********************/
/******************************************************/

/******************************************************/
/*******************  本地函数声明 *********************/
/******************************************************/

/******************************************************/
/*******************  外部函数引用 *********************/
/******************************************************/



/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
	* 2-修改GPIO的宏
  */
	
// 串口1-USART1
#define  pal_USARTx                   USART1
#define  pal_USART_CLK                RCC_APB2Periph_USART1
#define  pal_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  pal_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  pal_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  pal_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  pal_USART_TX_GPIO_PORT       GPIOA   
#define  pal_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  pal_USART_RX_GPIO_PORT       GPIOA
#define  pal_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  pal_USART_IRQ                USART1_IRQn
#define  pal_USART_IRQHandler         USART1_IRQHandler


// 串口2-USART2
//#define  pal_USARTx                   USART2
//#define  pal_USART_CLK                RCC_APB1Periph_USART2
//#define  pal_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  pal_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  pal_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  pal_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  pal_USART_TX_GPIO_PORT       GPIOA   
//#define  pal_USART_TX_GPIO_PIN        GPIO_Pin_2
//#define  pal_USART_RX_GPIO_PORT       GPIOA
//#define  pal_USART_RX_GPIO_PIN        GPIO_Pin_3

//#define  pal_USART_IRQ                USART2_IRQn
//#define  pal_USART_IRQHandler         USART2_IRQHandler

// 串口3-USART3
//#define  pal_USARTx                   USART3
//#define  pal_USART_CLK                RCC_APB1Periph_USART3
//#define  pal_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  pal_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  pal_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
//#define  pal_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  pal_USART_TX_GPIO_PORT       GPIOB   
//#define  pal_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  pal_USART_RX_GPIO_PORT       GPIOB
//#define  pal_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  pal_USART_IRQ                USART3_IRQn
//#define  pal_USART_IRQHandler         USART3_IRQHandler

// 串口4-UART4
//#define  pal_USARTx                   UART4
//#define  pal_USART_CLK                RCC_APB1Periph_UART4
//#define  pal_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  pal_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  pal_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
//#define  pal_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  pal_USART_TX_GPIO_PORT       GPIOC   
//#define  pal_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  pal_USART_RX_GPIO_PORT       GPIOC
//#define  pal_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  pal_USART_IRQ                UART4_IRQn
//#define  pal_USART_IRQHandler         UART4_IRQHandler


// 串口5-UART5
//#define  pal_USARTx                   UART5
//#define  pal_USART_CLK                RCC_APB1Periph_UART5
//#define  pal_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  pal_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
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

