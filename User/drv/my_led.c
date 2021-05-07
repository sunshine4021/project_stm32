/*******************************************************

* �ļ�����my_led.c

* ���ߣ�  luyangguang

* ������ led������ʼ��

* �޸��ˣ���¬���⡵

* �޸�ʱ�䣺2021-04-24

* �޸����ݣ�����
*******************************************************/

/*******************  ͷ�ļ����� ***********************/
/******************************************************/

#include "my_led.h"   

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

static void  my_led_gpio_config ( void );

/******************************************************/
/*******************  �ⲿ�������� *********************/
/******************************************************/


 /**
  * @brief  ���� LED �� GPIO ����
  * @param  ��
  * @retval ��
  */
static void my_led_gpio_config ( void )
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* ���� LED1 ���� */
	RCC_APB2PeriphClockCmd ( palLED1_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = palLED1_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init ( palLED1_GPIO_PORT, & GPIO_InitStructure );	

	/* ���� LED2 ���� */
	RCC_APB2PeriphClockCmd ( palLED2_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = palLED2_GPIO_PIN;	
	GPIO_Init ( palLED2_GPIO_PORT, & GPIO_InitStructure );	

	/* ���� LED3 ���� */
	RCC_APB2PeriphClockCmd ( palLED3_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = palLED3_GPIO_PIN;	
	GPIO_Init ( palLED3_GPIO_PORT, & GPIO_InitStructure );		
	  		
}


 /**
  * @brief  LED ��ʼ������
  * @param  ��
  * @retval ��
  */
void my_led_init ( void )
{
  my_led_gpio_config ();
	
	palLED1_OFF();
	palLED2_OFF();
	palLED3_OFF();
	
}



/*********************************************END OF FILE**********************/
