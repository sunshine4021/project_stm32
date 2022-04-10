/*******************************************************

* 文件名：my_master_task.c

* 作者：  luyangguang

* 描述： master逻辑主任务

* 修改人：〈卢阳光〉

* 修改时间：2021-04-24

* 修改内容：创建
*******************************************************/

/*******************  头文件包含 ***********************/
/******************************************************/

#include "my_master_task.h"

#include "my_master_main.h"

#include "my_log_main.h"

/******************************************************/
/*******************  全局变量定义 *********************/
/******************************************************/

static QueueHandle_t my_master_queue_handle;

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


/******************************************************************************
 * Function  -  my_master_task_main
 * Purpose   -  主任务while循环
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
		LOG_DEBUG("%s","welcome to FreeRTOS task run 10s cycle！");
	}	
}


/******************************************************************************
 * Function  -  my_master_task_get_queuue_handle
 * Purpose   -  获取master句柄，用来发消息
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
 * Purpose   -  master主任务初始化
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



