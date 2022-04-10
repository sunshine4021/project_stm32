/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	hwtimer.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-02-27
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		��Ƭ����ʱ����ʼ��
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��������
#include "my_mcu_timer.h"
#include "my_mcu_nvic.h"
#include "my_mcu_gpio.h"

//Ӳ������
#include "my_hwtimer.h"


/*
************************************************************
*	�������ƣ�	Timer1_8_PWM_Init
*
*	�������ܣ�	Timer1��8��PWM����
*
*	��ڲ�����	TIMx��TIM1 ���� TIM8
*				psc����Ƶֵ
*				arr������ֵ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Timer1_8_PWM_Init(TIM_TypeDef *TIMx, GPIO_TypeDef *gpio_group, unsigned short gpio_pin, unsigned short psc, unsigned short arr)
{
	
	MCU_GPIO_Init(gpio_group, gpio_pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, (void *)0);
	
	MCU_TIMER_Base_Init(TIMx, TIM_CKD_DIV1, TIM_CounterMode_Up, arr, psc, 0);
	
	MCU_Timer_PWM_Init(TIMx, TPC_4, TIM_OCMode_PWM1, TIM_OutputState_Enable, 0, 0, TIM_OCPolarity_Low, 0, 0, 0);
	
	TIM_Cmd(TIMx, ENABLE);										//ʹ��TIMx

}

/*
************************************************************
*	�������ƣ�	TIM3_PWM_Init
*
*	�������ܣ�	Timer3_PWM����
*
*	��ڲ�����	psc����Ƶֵ
*				arr������ֵ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void TIM3_PWM_Init(GPIO_TypeDef *gpio_group, unsigned short gpio_pin, unsigned short psc, unsigned short arr)
{
	
	MCU_GPIO_Init(gpio_group, gpio_pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, (void *)0);
	
	MCU_TIMER_Base_Init(TIM3, TIM_CKD_DIV1, TIM_CounterMode_Up, arr, psc, 0);
	
	MCU_Timer_PWM_Init(TIM3, TPC_4, TIM_OCMode_PWM2, TIM_OutputState_Enable, 0, 0, TIM_OCPolarity_Low, 0, 0, 0);
	
	TIM_Cmd(TIM3, ENABLE);										//ʹ��TIM3

}

/*
************************************************************
*	�������ƣ�	Timer6_7_Init
*
*	�������ܣ�	Timer6��7�Ķ�ʱ����
*
*	��ڲ�����	TIMx��TIM6 ���� TIM7
*				psc��Ƶֵ
*				arr������ֵ
*
*	���ز�����	��
*
*	˵����		timer6��timer7ֻ���и����жϹ���
************************************************************
*/
void Timer6_7_Init(TIM_TypeDef *TIMx, unsigned short psc, unsigned short arr)
{
	
	MCU_TIMER_Base_Init(TIMx, 0, TIM_CounterMode_Up, arr, psc, 0);
	
	if(TIMx == TIM6)
		MCU_NVIC_Init(TIM6_IRQn, ENABLE, 1, 1);
	else
		MCU_NVIC_Init(TIM7_IRQn, ENABLE, 1, 1);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);					//ʹ�ܸ����ж�
	
	TIM_Cmd(TIMx, ENABLE); 										//ʹ�ܶ�ʱ��

}

/*
************************************************************
*	�������ƣ�	TIM6_IRQHandler
*
*	�������ܣ�	RTOS��������ʱ�ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void TIM6_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}

}

/*
************************************************************
*	�������ƣ�	TIM7_IRQHandler
*
*	�������ܣ�	Timer7�����жϷ�����
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void TIM7_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}

}
