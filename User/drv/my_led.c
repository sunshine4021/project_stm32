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
#include "my_mcu_gpio.h"

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

 const static GPIO_LIST my_led_gpio_list[3] = {
													 {palLED1_GPIO_PORT, palLED1_GPIO_PIN, "LED1"},
													 {palLED2_GPIO_PORT, palLED2_GPIO_PIN, "LED2"},
													 {palLED3_GPIO_PORT, palLED3_GPIO_PIN, "LED3"},
											  };


 /**
  * @brief  LED 初始化函数
  * @param  无
  * @retval 无
  */
void my_led_init ( void )
{

	MCU_GPIO_Init(my_led_gpio_list.gpio_group[0],my_led_gpio_list.gpio_pin[0],GPIO_Mode_Out_PP,GPIO_Speed_50MHz,my_led_gpio_list[0].gpio_name);
	MCU_GPIO_Init(my_led_gpio_list.gpio_group[1],my_led_gpio_list.gpio_pin[1],GPIO_Mode_Out_PP,GPIO_Speed_50MHz,my_led_gpio_list[1].gpio_name);
	MCU_GPIO_Init(my_led_gpio_list.gpio_group[2],my_led_gpio_list.gpio_pin[2],GPIO_Mode_Out_PP,GPIO_Speed_50MHz,my_led_gpio_list[2].gpio_name);
	
	palLED1_OFF();
	palLED2_OFF();
	palLED3_OFF();
	
}



/*********************************************END OF FILE**********************/
