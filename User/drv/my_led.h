/*******************************************************

* �ļ�����my_led.h

* ���ߣ�  luyangguang

* ������ led������ʼ��

* �޸��ˣ���¬���⡵

* �޸�ʱ�䣺2021-04-24

* �޸����ݣ�����
*******************************************************/

/*******************  ͷ�ļ����� ***********************/
/******************************************************/

#ifndef __MY_LED_H_
#define	__MY_LED_H_

#include "stm32f10x.h"
#include "my_com.h" 

/******************************************************/
/*******************  ȫ�ֱ������� *********************/
/******************************************************/

/******************************************************/
/*******************  �������Ͷ��� *********************/
/******************************************************/

#define             palLED1_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             palLED1_GPIO_PORT                       GPIOB
#define             palLED1_GPIO_PIN                        GPIO_Pin_5

#define             palLED2_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             palLED2_GPIO_PORT                       GPIOB
#define             palLED2_GPIO_PIN                        GPIO_Pin_0

#define             palLED3_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             palLED3_GPIO_PORT                       GPIOB
#define             palLED3_GPIO_PIN                        GPIO_Pin_1



#define             palLED1_ON()                            GPIO_ResetBits ( palLED1_GPIO_PORT, palLED1_GPIO_PIN )
#define             palLED1_OFF()                           GPIO_SetBits ( palLED1_GPIO_PORT, palLED1_GPIO_PIN )
#define             palLED1_TOGGLE()                        GPIO_ReadOutputDataBit ( palLED1_GPIO_PORT, palLED1_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( palLED1_GPIO_PORT, palLED1_GPIO_PIN ) : GPIO_SetBits ( palLED1_GPIO_PORT, palLED1_GPIO_PIN )

#define             palLED2_ON()                            GPIO_ResetBits ( palLED2_GPIO_PORT, palLED2_GPIO_PIN )
#define             palLED2_OFF()                           GPIO_SetBits ( palLED2_GPIO_PORT, palLED2_GPIO_PIN )
#define             palLED2_TOGGLE()                        GPIO_ReadOutputDataBit ( palLED2_GPIO_PORT, palLED2_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( palLED2_GPIO_PORT, palLED2_GPIO_PIN ) : GPIO_SetBits ( palLED2_GPIO_PORT, palLED2_GPIO_PIN )

#define             palLED3_ON()                            GPIO_ResetBits ( palLED3_GPIO_PORT, palLED3_GPIO_PIN )
#define             palLED3_OFF()                           GPIO_SetBits ( palLED3_GPIO_PORT, palLED3_GPIO_PIN )
#define             palLED3_TOGGLE()                        GPIO_ReadOutputDataBit ( palLED3_GPIO_PORT, palLED3_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( palLED3_GPIO_PORT, palLED3_GPIO_PIN ) : GPIO_SetBits ( palLED3_GPIO_PORT, palLED3_GPIO_PIN )

/******************************************************/
/*******************  �ⲿ�������� *********************/
/******************************************************/

/******************************************************/
/*******************  ���غ������� *********************/
/******************************************************/

void  my_led_init( void );

/******************************************************/
/*******************  �ⲿ�������� *********************/
/******************************************************/
 


#endif /* __MY_LED_H_ */

