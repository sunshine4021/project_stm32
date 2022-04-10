/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	delay.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-01-23
	*
	*	版本： 		V1.0
	*
	*	说明： 		利用Timer4做阻塞式延时
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//delay头文件
#include "my_delay.h"


/*
************************************************************
*	函数名称：	Delay_Init
*
*	函数功能：	初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Delay_Init(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//STM32F10x单片机定时器的BUG，需要先启动一次，然后清除标志，以跳过这个BUG
	TIM4->CR1 = 1;
	while(!(TIM4->SR & 1));							//等待产生中断事件
	TIM4->SR &= ~(1UL << 0);						//清除标志
	TIM4->CR1 = 0;
	
	TIM4->CR1 |= 1UL << 3;							//单脉冲模式
	
	TIM4->EGR |= 1;									//更新生成
	
	TIM4->DIER = 0;									//禁止所有中断
	
	TIM4->CR1 &= (unsigned short)~TIM_CR1_CEN;		//停止计时
	
	TIM4->PSC = 71;

}

/*
************************************************************
*	函数名称：	DelayUs
*
*	函数功能：	微秒级延时
*
*	入口参数：	us：延时的时长
*
*	返回参数：	无
*
*	说明：		此时钟(21MHz)最大延时798915us
************************************************************
*/
void DelayUs(unsigned short us)
{

	TIM4->ARR = us;
	
	TIM4->PSC = 71;									//timer4为72MHz，设置为71+1分频，1MHz，1us计数一次
	
	TIM4->CR1 |= (unsigned short)TIM_CR1_CEN;		//开始计时
	
	while(!(TIM4->SR & 1));							//等待产生中断事件
	
	TIM4->SR &= ~(1UL << 0);						//清除标志

}

/*
************************************************************
*	函数名称：	DelayXms
*
*	函数功能：	毫秒级延时
*
*	入口参数：	ms：延时的时长
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void DelayXms(unsigned short ms)
{

	while(ms--)
		DelayUs(1000);

}
