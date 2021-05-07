/*******************************************************

* 文件名：my_led.c

* 作者：  luyangguang

* 描述： led驱动初始化

* 修改人：〈卢阳光〉

* 修改时间：2021-04-24

* 修改内容：创建
*******************************************************/

/*******************  头文件包含 ***********************/
/******************************************************/

#include "my_led.h"   

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

static void  my_led_gpio_config ( void );

/******************************************************/
/*******************  外部函数引用 *********************/
/******************************************************/


 /**
  * @brief  配置 LED 的 GPIO 功能
  * @param  无
  * @retval 无
  */
static void my_led_gpio_config ( void )
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* 配置 LED1 引脚 */
	RCC_APB2PeriphClockCmd ( palLED1_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = palLED1_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init ( palLED1_GPIO_PORT, & GPIO_InitStructure );	

	/* 配置 LED2 引脚 */
	RCC_APB2PeriphClockCmd ( palLED2_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = palLED2_GPIO_PIN;	
	GPIO_Init ( palLED2_GPIO_PORT, & GPIO_InitStructure );	

	/* 配置 LED3 引脚 */
	RCC_APB2PeriphClockCmd ( palLED3_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = palLED3_GPIO_PIN;	
	GPIO_Init ( palLED3_GPIO_PORT, & GPIO_InitStructure );		
	  		
}


 /**
  * @brief  LED 初始化函数
  * @param  无
  * @retval 无
  */
void my_led_init ( void )
{
  my_led_gpio_config ();
	
	palLED1_OFF();
	palLED2_OFF();
	palLED3_OFF();
	
}



/*********************************************END OF FILE**********************/
