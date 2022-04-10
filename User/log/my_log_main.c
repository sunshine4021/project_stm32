/*******************************************************

* 文件名：my_log_main.h

* 作者：  luyangguang

* 描述： 调试日记输出

* 修改人：〈卢阳光〉

* 修改时间：2021-04-24

* 修改内容：创建
*******************************************************/

/*******************  头文件包含 ***********************/
/******************************************************/
#include "my_log_main.h"

#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

#include "my_log_main.h"
#include "my_drv_task.h"
#include "my_master_task.h"

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

/******************************************************************************
 * Function  -  my_log_send_to_uart
 * Purpose   -  发送消息给drv任务把数据发送出去
 * Params 
 *   @uint8_t *data  数据缓冲器
 *   @uint16_t data_len 数据的长度
 * Return
 *   @  
 * Limitation -   
 * ----------------------------------------
 * Modification history
 * luyangguang create  @2022-04-10
 ******************************************************************************/
static void my_log_send_to_uart(uint8_t *data , uint16_t data_len)
{
		my_task_message_struct message = {0};
		uint8_t *send_data = NULL;
		
		send_data = (uint8_t *) pvPortMalloc(data_len + 1);
		
		memset(send_data,0,data_len + 1);
		memcpy(send_data,data,data_len);
						
		message.mes_id = MY_DRV_UART_DATA_SEND;
		message.data_len = data_len;
		message.pvdata = send_data;
		message.param_1 = 1;
		
		xQueueSendToBack(my_drv_task_get_queuue_handle(),&message,0);

		//发送失败要判断的，然后释放资源

}
	

/******************************************************************************
 * Function  -  my_pre_log
 * Purpose   -  格式化LOG打印输出
 * Params 
 *   @ void
 * Return
 *   @  
 * Limitation -   
 * ----------------------------------------
 * Modification history
 * luyangguang create  @2022-04-10
 ******************************************************************************/
void my_pre_log(const char *prefix,const char *format,...)
{
		char line[1024] = {0};	
	  va_list args;
		strncpy(line,prefix,strlen(prefix));
	  va_start(args,format);
		vsnprintf(line + strlen(line),1024 - strlen(line),format,args);	
	  va_end(args);
		my_log_send_to_uart((uint8_t *)line,strlen(line));		
}




