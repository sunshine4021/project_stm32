/*******************************************************

* �ļ�����main.c

* ���ߣ�  luyangguang

* ������ STM32 FreeRTOS�Ĺ���ģ��

* �޸��ˣ���¬���⡵

* �޸�ʱ�䣺2021-04-24

* �޸����ݣ�����
*******************************************************/

/*******************  ͷ�ļ����� ***********************/
/******************************************************/
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "my_drv_task.h"
#include "my_master_task.h"

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




/******************************************************
* ������ : main
* ������������ : ����Ŀ�ʼ�����������
* �������� : 
* ��������ֵ : 
* ���� : luyangguang
* �汾 : 1.0
* ��ʷ�汾 : ��
*******************************************************/
int main ( void )
{	

	xTaskCreate( my_drv_task_init, "MY Task DRV", 512, NULL, 1, NULL );
	
	xTaskCreate( my_master_task_init, "MY Task MASTER", 512, NULL, 1, NULL );
	
  vTaskStartScheduler();
	
	return 0;
}



/*********************************************END OF FILE**********************/











