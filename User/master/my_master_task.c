/*******************************************************

* �ļ�����my_master_task.c

* ���ߣ�  luyangguang

* ������ master�߼�������

* �޸��ˣ���¬���⡵

* �޸�ʱ�䣺2021-04-24

* �޸����ݣ�����
*******************************************************/

/*******************  ͷ�ļ����� ***********************/
/******************************************************/

#include "my_master_task.h"

#include "my_master_main.h"

#include "my_log_main.h"

/******************************************************/
/*******************  ȫ�ֱ������� *********************/
/******************************************************/

static QueueHandle_t my_master_queue_handle;

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


/******************************************************************************
 * Function  -  my_master_task_main
 * Purpose   -  ������whileѭ��
 * Params 
 *   @ void
 * Return
 *   @  
 * Limitation -   
 * ----------------------------------------
 * Modification history
 * luyangguang create  @2022-04-10
 ******************************************************************************/
static void my_master_task_main(void)
{
	my_task_message_struct drv_message = {0};

	while(1)
	{
		if(xQueueReceive(my_master_queue_handle,&drv_message,10000))
		{
			switch(drv_message.mes_id)
			{
				case MY_MASTER_UART_DATA_RECV:
				{	
					MY_MASTER_UART_DATA_RECV_handle(&drv_message);
					LOG_DEBUG("[%d]%s",drv_message.data_len,drv_message.pvdata);
					vPortFree(drv_message.pvdata);
				}
				break;
				default:
					break;
			}
		}	
		LOG_DEBUG("%s","welcome to FreeRTOS task run 10s cycle��");
	}	
}


/******************************************************************************
 * Function  -  my_master_task_get_queuue_handle
 * Purpose   -  ��ȡmaster�������������Ϣ
 * Params 
 *   @ void
 * Return
 *   @   
 * Limitation -   
 * ----------------------------------------
 * Modification history
 * luyangguang create  @2022-04-10
 ******************************************************************************/
QueueHandle_t my_master_task_get_queuue_handle(void)
{
	return my_master_queue_handle;
}


/******************************************************************************
 * Function  -  my_master_task_init
 * Purpose   -  master�������ʼ��
 * Params 
 *   @void * pvParameters
 * Return
 *   @  
 * Limitation -   
 * ----------------------------------------
 * Modification history
 * luyangguang create  @2022-04-10
 ******************************************************************************/
void my_master_task_init( void * pvParameters )
{	
	my_master_queue_handle = xQueueCreate(50,sizeof(my_task_message_struct));
	my_master_task_main();
		
}



